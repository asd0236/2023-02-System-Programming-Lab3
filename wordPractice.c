#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_SENTENCES 5
#define MAX_SENTENCE_LENGTH 100

// 랜덤 문장 배열
const char *sentences[MAX_SENTENCES] = {
    "The quick brown fox jumps over the lazy dog.",
    "Programming is fun and challenging.",
    "Practice makes perfect.",
    "C programming is powerful.",
    "Coding is like solving puzzles."
};

// 대문자로 변환하는 함수
void toUpperCase(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int totalTypos = 0;
    time_t startTime, endTime;

    printf("타자 연습을 시작합니다. 아무 키를 누르면 시작됩니다.\n");
    getchar();

    for (int i = 0; i < MAX_SENTENCES; ++i) {
        // 문장 선택
        const char *sentence = sentences[i];

        // 대문자로 변환
        char upperCaseSentence[MAX_SENTENCE_LENGTH];
        strcpy(upperCaseSentence, sentence);
        toUpperCase(upperCaseSentence);

        printf("\n%s\n", sentence);

        // 시작 시간 기록
        startTime = time(NULL);

        // 사용자 입력 받기
        char userInput[MAX_SENTENCE_LENGTH];
        fgets(userInput, sizeof(userInput), stdin);
        toUpperCase(userInput);

        // 종료 시간 기록
        endTime = time(NULL);

        // 오타 체크
        int typos = 0;
        for (int j = 0; j < strlen(upperCaseSentence); ++j) {
            if (upperCaseSentence[j] != userInput[j]) {
                typos++;
            }
        }

        totalTypos += typos;

        // 결과 출력
        printf("\n오타 횟수: %d\n", typos);
        printf("소요 시간: %ld초\n", endTime - startTime);
    }

    // 전체 평균 분당 타자수 계산
    int totalCharacters = 0;
    for (int i = 0; i < MAX_SENTENCES; ++i) {
        totalCharacters += strlen(sentences[i]);
    }

    double averageTypingSpeed = (double)totalCharacters / (endTime - startTime) * 60;
    double averageTypos = (double)totalTypos / MAX_SENTENCES;

    printf("\n전체 오타 횟수: %d\n", totalTypos);
    printf("전체 평균 분당 타자수: %.2f\n", averageTypingSpeed);
    printf("전체 평균 오타 횟수: %.2f\n", averageTypos);

    return 0;
}

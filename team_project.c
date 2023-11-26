#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

// 함수 원형 추가
int execute_command(char **argv, int background);

int main() {
    char buf[256];
    char *argv[50];
    int narg;
    pid_t pid;
    int background = 0;  // 백그라운드 실행 여부를 나타내는 플래그

    while (1) {
        printf("shell> ");
        fgets(buf, sizeof(buf), stdin);

        // 개행 문자 제거
        buf[strcspn(buf, "\n")] = '\0';

        // "exit" 입력 시 프로그램 종료
        if (strcmp(buf, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // 백그라운드 실행 여부 확인
        if (buf[strlen(buf) - 1] == '&') {
            background = 1;
            buf[strlen(buf) - 1] = '\0';  // '&' 제거
        } else {
            background = 0;
        }

        narg = getargs(buf, argv);
        pid = fork();

        if (pid == 0) {
            // 자식 프로세스에서 명령 실행
            int result = execute_command(argv, background);

            // 에러 발생 시 종료
            if (result == -1) {
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            // 부모 프로세스에서 대기
            if (background == 0) {
                wait(NULL);
            }
        } else {
            perror("fork failed");
        }
    }

    return 0;
}

int execute_command(char **argv, int background) {
    // 신호 처리를 위한 구조체
    struct sigaction act;

    // 시그널 핸들러 설정
    act.sa_handler = SIG_DFL;
    act.sa_flags = SA_NOCLDSTOP;

    // 시그널 핸들러 등록
    sigaction(SIGCHLD, &act, NULL);

    if (execvp(argv[0], argv) == -1) {
        perror("execvp");
        return -1;
    }

    return 0;
}

int getargs(char *cmd, char **argv) {
    int narg = 0;
    while (*cmd) {
        if (*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
                cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}

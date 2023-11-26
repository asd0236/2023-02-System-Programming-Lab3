#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int my_system(const char *command) {
    pid_t pid;
    int status;

    if (command == NULL) {
        fprintf(stderr, "Command is NULL.\n");
        return -1;
    }

    // 새로운 프로세스 생성
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // 자식 프로세스
        // 쉘을 통해 명령 실행
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {  // 부모 프로세스
        // 자식 프로세스의 종료를 기다림
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // 자식 프로세스의 종료 상태 반환
        return status;
    }
}

int main() {
    char command[100];

    printf("Enter a command: ");
    fgets(command, sizeof(command), stdin);

    // 개행 문자 제거
    command[strcspn(command, "\n")] = '\0';

    // my_system 함수 호출
    int result = my_system(command);

    printf("Command returned status: %d\n", result);

    return 0;
}

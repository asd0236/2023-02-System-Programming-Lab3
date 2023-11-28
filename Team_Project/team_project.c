#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int execute_command(char **argv, int background);
int getargs(char *cmd, char **argv);

void sigint_handler(int signo)
{
    printf("Ctrl+C 프로세스 종료\n");
}

void sigquit_handler(int signo)
{
    printf("Ctrl+Z: 프로세스 일시 중지\n");
}

int main()
{
    char buf[256];
    char *argv[50];
    int narg;
    pid_t pid;
    int background = 0;
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);

    while (1)
    {
        printf("shell> ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, "exit") == 0)
        {
            printf("Goodbye!\n");
            break;
        }

        // 파일 재지향 또는 파이프 여부 확인
        int redirect_output = 0;
        int redirect_input = 0;
        int pipe_cmd = 0;
        char *output_file = NULL;
        char *input_file = NULL;

        char *token = strtok(buf, " ");
        while (token != NULL)
        {
            if (strcmp(token, ">") == 0)
            {
                redirect_output = 1;
                token = strtok(NULL, " ");
                if (token != NULL)
                {
                    output_file = token;
                }
            }
            else if (strcmp(token, "<") == 0)
            {
                redirect_input = 1;
                token = strtok(NULL, " ");
                if (token != NULL)
                {
                    input_file = token;
                }
            }
            else if (strcmp(token, "|") == 0)
            {
                pipe_cmd = 1;
            }
            else
            {
                argv[narg++] = token;
            }
            token = strtok(NULL, " ");
        }

        argv[narg] = NULL;

        if (pipe_cmd)
        {
            // 파이프 기능 추가
            // 파이프를 사용하여 두 명령어를 연결할 수 있습니다.
            // 여기서는 단순히 하나의 명령어를 실행하는 예제를 제공합니다.
            // 두 명령어를 연결하려면 파이프를 사용하여 두 개의 자식 프로세스를 생성하고,
            // 하나는 왼쪽 명령어를 실행하고 다른 하나는 오른쪽 명령어를 실행하면 됩니다.
            // 결과를 파이프로 연결하여 데이터를 전달할 수 있습니다.
            printf("파이프 기능은 여기서는 다루지 않습니다.\n");
        }
        else if (redirect_output)
        {
            // 파일 출력 재지향 기능 추가
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd == -1)
            {
                perror("open");
            }
            else
            {
                pid = fork();
                if (pid == 0)
                {
                    // 자식 프로세스에서 명령 실행 및 파일 재지향
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    execute_command(argv, background);
                    exit(EXIT_SUCCESS);
                }
                else if (pid > 0)
                {
                    if (background == 0)
                    {
                        wait(NULL);
                    }
                }
                else
                {
                    perror("fork failed");
                }
            }
        }
        else if (redirect_input)
        {
            // 파일 입력 재지향 기능 추가
            int fd = open(input_file, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
            }
            else
            {
                pid = fork();
                if (pid == 0)
                {
                    // 자식 프로세스에서 명령 실행 및 파일 재지향
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                    execute_command(argv, background);
                    exit(EXIT_SUCCESS);
                }
                else if (pid > 0)
                {
                    if (background == 0)
                    {
                        wait(NULL);
                    }
                }
                else
                {
                    perror("fork failed");
                }
            }
        }
        else
        {
            // 일반적인 명령 실행
            pid = fork();
            if (pid == 0)
            {
                execute_command(argv, background);
                exit(EXIT_SUCCESS);
            }
            else if (pid > 0)
            {
                if (background == 0)
                {
                    wait(NULL);
                }
            }
            else
            {
                perror("fork failed");
            }
        }

        narg = 0;
    }

    return 0;
}

int execute_command(char **argv, int background)
{
    struct sigaction act;
    act.sa_handler = SIG_DFL;
    act.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act, NULL);

    if (execvp(argv[0], argv) == -1)
    {
        perror("execvp");
        return -1;
    }

    return 0;
}

int getargs(char *cmd, char **argv)
{
    int narg = 0;
    while (*cmd)
    {
        if (*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else
        {
            argv[narg++] = cmd++;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
                cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}

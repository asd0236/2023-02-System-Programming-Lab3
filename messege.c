#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_MESSAGE_SIZE 256

// 메시지 구조체
struct message {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};

// 메시지 큐 생성 및 반환
int create_message_queue() {
    key_t key = ftok(".", 'q');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    return msgid;
}

// 메시지 전송
void send_message(int msgid, long mtype, const char *mtext) {
    struct message msg;
    msg.mtype = mtype;
    strncpy(msg.mtext, mtext, MAX_MESSAGE_SIZE - 1);

    if (msgsnd(msgid, (void *)&msg, sizeof(struct message) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

// 메시지 수신
void receive_message(int msgid, long mtype, char *mtext) {
    struct message msg;

    if (msgrcv(msgid, (void *)&msg, sizeof(struct message) - sizeof(long), mtype, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    strncpy(mtext, msg.mtext, MAX_MESSAGE_SIZE - 1);
}

int main() {
    int msgid = create_message_queue();
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // 자식 프로세스 (수신)
        while (1) {
            char received_message[MAX_MESSAGE_SIZE];
            receive_message(msgid, 1, received_message);
            printf("Received: %s\n", received_message);
        }
    } else { // 부모 프로세스 (전송)
        while (1) {
            char input_message[MAX_MESSAGE_SIZE];
            printf("Enter message: ");
            fgets(input_message, MAX_MESSAGE_SIZE, stdin);
            send_message(msgid, 1, input_message);
        }
    }

    return 0;
}

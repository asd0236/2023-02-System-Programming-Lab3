#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


#define SHM_SIZE 1024

// 공유 메모리 구조체
struct shared_memory {
    int data_size;
    char data[SHM_SIZE];
};

// 세마포어 구조체
struct sembuf sem_lock = {0, -1, SEM_UNDO};
struct sembuf sem_unlock = {0, 1, SEM_UNDO};

// 세마포어를 사용하여 공유 메모리에 데이터 복사
void copy_data(int shm_id, int sem_id, const char *source_filename, const char *dest_filename) {
    struct shared_memory *shm_ptr;

    // 세마포어를 이용하여 잠금
    semop(sem_id, &sem_lock, 1);

    // 파일을 읽어 공유 메모리에 저장
    FILE *source_file = fopen(source_filename, "rb");
    if (source_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fseek(source_file, 0, SEEK_END);
    long file_size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    shm_ptr = (struct shared_memory *)shmat(shm_id, NULL, 0);
    if ((long)shm_ptr == -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shm_ptr->data_size = file_size;
    fread(shm_ptr->data, 1, SHM_SIZE, source_file);

    // 세마포어를 이용하여 잠금 해제
    semop(sem_id, &sem_unlock, 1);

    fclose(source_file);
}

// 세마포어를 사용하여 공유 메모리에서 데이터 읽기
void read_data(int shm_id, int sem_id, const char *dest_filename) {
    struct shared_memory *shm_ptr;

    // 세마포어를 이용하여 잠금
    semop(sem_id, &sem_lock, 1);

    // 파일에 데이터 쓰기
    FILE *dest_file = fopen(dest_filename, "wb");
    if (dest_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    shm_ptr = (struct shared_memory *)shmat(shm_id, NULL, 0);
    if ((long)shm_ptr == -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    fwrite(shm_ptr->data, 1, shm_ptr->data_size, dest_file);

    // 세마포어를 이용하여 잠금 해제
    semop(sem_id, &sem_unlock, 1);

    fclose(dest_file);
}

int main() {
    // 공유 메모리 생성
    key_t shm_key = ftok(".", 's');
    int shm_id = shmget(shm_key, sizeof(struct shared_memory), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // 세마포어 생성
    key_t sem_key = ftok(".", 'e');
    int sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // 세마포어 초기화
    semctl(sem_id, 0, SETVAL, 1);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // 자식 프로세스 (쓰기)
        copy_data(shm_id, sem_id, "source.txt", "dest.txt");
    } else { // 부모 프로세스 (읽기)
        wait(NULL);
        read_data(shm_id, sem_id, "dest.txt");
    }

    // 공유 메모리 및 세마포어 삭제
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}

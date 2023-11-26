#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void visit_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            // 디렉토리인 경우 재귀적으로 방문
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("Directory: %s\n", entry->d_name);
                char new_path[PATH_MAX];
                snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
                visit_directory(new_path);
            }
        } else {
            // 파일인 경우 출력
            printf("File: %s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main() {
    const char *current_dir = ".";
    printf("Listing files and directories in '%s':\n", current_dir);
    visit_directory(current_dir);

    return 0;
}

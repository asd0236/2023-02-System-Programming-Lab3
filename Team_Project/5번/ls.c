#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // 디렉터리 핸들링을 위한 헤더 파일

int main()
{
  struct dirent *entry;
  DIR *dp;

  // 현재 디렉터리 열기
  dp = opendir(".");

  if (dp == NULL)
  {
    perror("opendir");
    exit(1);
  }

  // 디렉터리 내 파일 및 디렉터리 목록 출력
  while ((entry = readdir(dp)))
  {
    printf("%s\n", entry->d_name);
  }

  closedir(dp);

  return 0;
}
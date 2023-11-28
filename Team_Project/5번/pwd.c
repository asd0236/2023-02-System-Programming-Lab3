#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  char *cwd;
  size_t size;

  // 현재 작업 디렉터리 경로 가져오기
  size = pathconf(".", _PC_PATH_MAX);

  if ((cwd = (char *)malloc((size_t)size)) != NULL)
  {
    if (getcwd(cwd, (size_t)size) != NULL)
    {
      printf("%s\n", cwd);
    }
    else
    {
      perror("getcwd");
    }
    free(cwd);
  }
  else
  {
    perror("malloc");
  }

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
    exit(1);
  }

  const char *dir_name = argv[1];

  if (mkdir(dir_name, 0777) == 0)
  {
    printf("Directory '%s' created successfully.\n", dir_name);
  }
  else
  {
    perror("mkdir");
    exit(1);
  }

  return 0;
}
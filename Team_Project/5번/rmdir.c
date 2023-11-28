#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
    exit(1);
  }

  const char *dir_name = argv[1];

  if (rmdir(dir_name) == 0)
  {
    printf("Directory '%s' removed successfully.\n", dir_name);
  }
  else
  {
    perror("rmdir");
    exit(1);
  }

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <source_file> <link_name>\n", argv[0]);
    exit(1);
  }

  const char *source_file = argv[1];
  const char *link_name = argv[2];

  if (link(source_file, link_name) == 0)
  {
    printf("Hard link created: %s -> %s\n", link_name, source_file);
  }
  else
  {
    perror("link");
    exit(1);
  }

  return 0;
}
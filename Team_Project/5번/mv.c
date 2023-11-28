#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s source_file target_path\n", argv[0]);
    exit(1);
  }

  char *source_file = argv[1];
  char *target_path = argv[2];

  if (rename(source_file, target_path) != 0)
  {
    perror("Unable to move/rename the file");
    exit(1);
  }

  printf("File '%s' moved/renamed to '%s' successfully.\n", source_file, target_path);
  return 0;
}

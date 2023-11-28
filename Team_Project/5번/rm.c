#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s file_to_delete\n", argv[0]);
    exit(1);
  }

  char *file_to_delete = argv[1];

  if (remove(file_to_delete) != 0)
  {
    perror("Unable to delete the file");
    exit(1);
  }

  printf("File '%s' deleted successfully.\n", file_to_delete);
  return 0;
}
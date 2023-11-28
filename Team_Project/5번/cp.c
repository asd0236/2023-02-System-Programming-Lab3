#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
    exit(1);
  }

  char *source_file = argv[1];
  char *destination_file = argv[2];

  FILE *source = fopen(source_file, "rb");
  if (source == NULL)
  {
    perror("Unable to open source file");
    exit(1);
  }

  FILE *destination = fopen(destination_file, "wb");
  if (destination == NULL)
  {
    perror("Unable to create destination file");
    fclose(source);
    exit(1);
  }

  char buffer[1024];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0)
  {
    fwrite(buffer, 1, bytes_read, destination);
  }

  fclose(source);
  fclose(destination);

  printf("File copied successfully.\n");
  return 0;
}
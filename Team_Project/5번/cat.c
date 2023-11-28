#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s file1 [file2 ...]\n", argv[0]);
    exit(1);
  }

  for (int i = 1; i < argc; i++)
  {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL)
    {
      perror("Unable to open file");
      continue;
    }

    int c;
    while ((c = fgetc(file)) != EOF)
    {
      putchar(c);
    }

    fclose(file);
  }

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage : ./recover filename\n");
        return 1;
    }
    char *fname1 = argv[1];
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 4;
    }
    int x = 512;
    int c = 0;
    FILE *file2;
    while (x != 0)
    {
        BYTE buff[x];
        x = fread(&buff, 1, x, file);
        if (buff[0] == 0xff && buff[1] == 0xd8 && buff[2] == 0xff && (buff[3] & 0xf0) == 0xe0)
        {
            if (c == 0)
            {
                char *fname = malloc(8);
                sprintf(fname, "%03i.jpg", c);
                file2 = fopen(fname, "w");
                fwrite(&buff, 1, x, file2);
                c++;
            }
            else
            {
                fclose(file2);
                char *fname = malloc(8);
                sprintf(fname, "%03i.jpg", c);
                file2 = fopen(fname, "w");
                fwrite(&buff, 1, x, file2);
                c++;
            }
        }
        else if (c > 0)
        {
            fwrite(&buff, 1, x, file2);
        }
    }
    fclose(file);
    fclose(file2);
}


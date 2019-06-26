// Problem Set 3, Recover, 26 June 2019

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure one command-line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image ");
        return 1;
    }

    // remember input filename
    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    BYTE buffer[512];
    int i = 0;
    char filename[8];
    FILE *img = NULL;

    // Iterating over infile every 512 bytes
    while (fread(buffer, 512, 1, inptr))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If is no longer the first image recovered
            if (i > 0)
            {
                fclose(img);
            }
            // Create a new jpeg file
            sprintf(filename, "%03i.jpg", i);
            img = fopen(filename, "w");
            i++;
        }
        // Only start writing into a new image only once the first image has been found
        if (i > 0 && img != NULL)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    fclose(inptr);
    fclose(img);

    return 0;

}

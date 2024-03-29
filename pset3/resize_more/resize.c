// Problem Set 3, resize, more comfortable, 25 June 2019
// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    // remember scale factor and filenames
    float n;
    sscanf(argv[1], "%f", &n);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (n <= 0.0 || n > 100.0)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_out;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_out;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    bf_out = bf;
    bi_out = bi;
    float outWidth = bi_out.biWidth * n;
    float outHeight = bi_out.biHeight * n;

    bi_out.biWidth = round(outWidth);
    bi_out.biHeight = round(outHeight);

    if (bi_out.biWidth == 0)
    {
        bi_out.biWidth = 1;
    }

    if (bi_out.biHeight == 0)
    {
        bi_out.biHeight = 1;
    }

    // determine padding for scanlines
    int padding_in = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_out = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi_out.biSizeImage = (bi_out.biWidth * 3 + padding_out) * abs(bi_out.biHeight);
    bf_out.bfSize = bi_out.biSizeImage + 54;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    if (n >= 1)
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // iterate over pixels in scanline
                for (int k = 0; k < bi.biWidth; k++)
                {
                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    for (int l = 0; l < n; l++)
                    {
                        // write RGB triple to outfile
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
                // then add it back (to demonstrate how)
                for (int m = 0; m < padding_out; m++)
                {
                    fputc(0x00, outptr);
                }
                if (j < n - 1)
                {
                    fseek(inptr, -bi.biWidth * 3, SEEK_CUR);
                }
            }
            // skip over padding, if any
            fseek(inptr, padding_in, SEEK_CUR);
        }
    }
    else
    {
        // iterate over infile's scanlines
        for (int i = 1, biHeight = abs(bi_out.biHeight); i <= biHeight; i++)
        {
            int diffw = bi.biWidth - bi_out.biWidth;
            int skippixelw1 = ceil(bi_out.biWidth / diffw);
            int skippixelw2 = ceil(1.0 / skippixelw1);

            int diffh = abs(bi.biHeight) - abs(bi_out.biHeight);
            int skipscanh1 = ceil(abs(bi_out.biHeight) / diffh);

            // iterate over pixels in scanline
            for (int j = 1; j <= bi_out.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                if (j % skippixelw1 == 0)
                {
                    // skip extra pixels from infile
                    fseek(inptr, (skippixelw2 * sizeof(RGBTRIPLE)), SEEK_CUR);
                }

            }
            // then add it back (to demonstrate how)
            for (int m = 0; m < padding_out; m++)
            {
                fputc(0x00, outptr);
            }
            // skip over padding, if any
            fseek(inptr, padding_in, SEEK_CUR);
            // skip extra scanlines from infile
            if (i % skipscanh1 == 0)
            {
                fseek(inptr, (abs(bi.biHeight) * sizeof(RGBTRIPLE) + padding_in), SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// Problem Set 3, whodunit written questions, 24 June 2019
# Questions

## What's `stdint.h`?

A header file in the C Standard Library that provides a set of typedefs specifying exact-width integer types, together with defined minimum and maximum allowable values for each type, using macros.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

If only using `int`, the integer's size can be a variable (but >=16 bits). On some systems it might result in 16-bit integer by compiler, on others might be 32 or 64 bits. Using the above mentioned fixed-width integer types can ensure that the integers always have the same width everywhere. 
To accommodate some file and communicationprotocol formats.
When high degree of portability between platforms is required over platforms.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1, 4, 4, 2, respectively.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The ASCII characters 'B' followed by 'M'.

## What's the difference between `bfSize` and `biSize`?

bfSize: the size, in bytes, of the bitmap file, including the headerfiles.
biSize: the number of bytes required by the structure (the BITMAPINFOHEADER header file, 40bytes)

## What does it mean if `biHeight` is negative?

`biHeight`: the height of bitmap, in pixel.
The bitmap is a top-down DIB, and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

Could either because the file with the indicated filename doesn't exist, or is due to permission issues.

## Why is the third argument to `fread` always `1` in our code?

Because BITMAPFILEHEADER, BITMAPINFOHEADER, and RGBTRIPLE are themselves each typedef struct-ed as a single type, a single entity. Therefore there is only one element from each of them for us to read; that is, the whole BITMAPFILEHEADEAR, BITMAPINFOHEADER, and RGBTRIPLE, respectively.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

Allows the user to rewind or fast-forward within a file.
Move file pointer associated with a given file to a specific location.

## What is `SEEK_CUR`?

The third argument to `fseek` defines the point with respect to which the file pointer needs to be moved. / Position used as reference for the offset.
SEEK_CUR: file pointer's current position

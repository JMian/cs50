# Problem Set 6, Sentimental-Vigenere, Python, 14 July 2019

from cs50 import get_string
import sys
from sys import argv


def main():
    # Check how many command line arguments there are
    if len(argv) != 2:
        print("Usage: python vigenere.py str(alpha)")
        sys.exit(1)

    # Check if the command line argument is composed entirely of alphabets
    if argv[1].isalpha() == False:
        print("Usage: python vigenere.py str(alpha)")
        sys.exit(1)

    keyword = argv[1]

    plaintext = get_string("plaintext: ")
    print("ciphertext: ", end="")

    # Get teh lengths of plaintext and keyword
    nk = len(keyword)

    # To keep track of the index of the keyword
    j = 0

    # Iterate and rotate each character from the plaintext
    for c in plaintext:

        # If the current index of the keyword equals to the length of the keyword, start over using the keyword
        if j >= nk:
            j = 0

        k = shift(keyword[j])
        # If uppercase letter
        if c.isupper() == True:
            print((chr((ord(c) + k - 65) % 26 + 65)), end="")
            j += 1
        # If lowercase letter
        elif c.islower() == True:
            print((chr((ord(c) + k - 97) % 26 + 97)), end="")
            j += 1
        else:
            print(c, end="")

    print()


def shift(ck):
    if ck.isupper() == True:
        shiftvalue = ord(ck) - 65
    elif ck.islower() == True:
        shiftvalue = ord(ck) - 97
    return shiftvalue


if __name__ == "__main__":
    main()
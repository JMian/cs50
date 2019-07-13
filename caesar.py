# Problem Set 6, Sentimental-Caesar, Python, 14 July 2019

from cs50 import get_string
import sys
from sys import argv

# Check how many command line arguments there are
if len(argv) != 2:
    print("Usage: python caesar.py k")
    sys.exit(1)

# Check if the command line argument is a positive integer
if argv[1].isdigit() == False:
    print("is not digit")
    sys.exit(1)

k = int(argv[1])

plaintext = get_string("plaintext: ")
print("ciphertext: ", end="")

# Iterate and rotate each character from the plaintext
for c in plaintext:
    # If uppercase letter
    if c.isupper() == True:
        print((chr((ord(c) + k - 65) % 26 + 65)), end="")
    # If lowercase letter
    elif c.islower() == True:
        print((chr((ord(c) + k - 97) % 26 + 97)), end="")
    else:
        print(c, end="")
print()
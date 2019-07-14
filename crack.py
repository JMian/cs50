# Problem Set 6, Sentimental-Crack, Python, 14 July 2019

import crypt
import itertools
import sys
from sys import argv

# Check how many command line arguments there are
if len(argv) != 2:
    print("Usage: python crack.py hash")
    sys.exit(1)

hashpw = argv[1]

# Get the first two value of the input hash
salt = hashpw[:2]

for i in range(1, 6, 1):
    pwrange = itertools.product('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz', repeat=i)

    # Get individual pack from the list of lists resulted from itertools.product
    for pwchar in pwrange:

        # Join the individual element in the list(pack) to become a string
        pw = "".join(pwchar)

        # Check hash
        if crypt.crypt(pw, salt) == hashpw:
            print(pw)
            sys.exit(0)
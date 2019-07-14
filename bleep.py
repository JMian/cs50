# Problem Set 6, Bleep, Python, 14 July 2019

from cs50 import get_string
from sys import argv
import sys
import os.path


def main():

    # TODO
    # Check the command-line argument
    if len(argv) != 2:
        print("Usage: python bleep.py textfile")
        sys.exit(1)

    banfile = argv[1]

    # Open the user-input-file if it does exist, else exit
    if os.path.exists(banfile):
        file = open(banfile, "r")
    else:
        print(f"Could not load {banfile}")
        sys.exit(1)

    # Create a set to store the banned words inside the textfile imported
    banwords = set()

    # Load the banned words from the textfile to the banwords set for comparing
    for line in file:
        banwords.add(line.rstrip("\n"))
    file.close()

    # Prompt user for message to be censored
    inmessage = get_string("What message would you like to censor?\n")

    # Split the entire message into individual words
    indimessage = inmessage.split()

    # Access the individual word from the set containing the individual words of the message
    for i in range(len(indimessage)):
        inword = indimessage[i]

        # Check if the individual word is one of the banned words
        if inword.lower() in banwords:
            # Replace the banned words, if exists inside the message, with *
            print("*" * len(inword), end=" ")
        else:
            print(inword, end=" ")
    print()


if __name__ == "__main__":
    main()
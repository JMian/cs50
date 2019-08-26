# Problem Set 6, Sentimental-Mario(more), Python, 13 July 2019

from cs50 import get_int

# Prompt user for a positive height no more than 8
while True:
    n = get_int("Height: ")
    if n > 0 and n < 9:
        break

# Print out the pyramid
for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 1), end="")
    print(" " * 2, end="")
    print("#" * (i + 1))

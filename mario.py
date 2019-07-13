# Problem Set 6, Sentimental-Mario(less), Python, 13 July 2019

from cs50 import get_int

# Prompt user for a positive height no more than 8
while True:
    n = get_int("Height: ")
    if n > 0 and n < 9:
        break

for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 1), end="")
    print()
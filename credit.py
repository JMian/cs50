# Problem Set 6, Sentimental-Credit, Python, 13 July 2019

from cs50 import get_int
import sys

while True:
    number = get_int("Number: ")
    if number >= 0:
        break

# Check the length of the number
n = len(str(number))
if n != 13 and n != 15 and n != 16:
    print("INVALID")
    sys.exit(0)

mod = 0
mod_mod = 0
checksum = 0
first2dg = 0
digit = 0

# Iterating over each digit of the input number
while number >= 1:
    mod = number % 10
    number //= 10
    digit += 1

    # If it is every other digit, starting with the number's second-to-last digit
    if digit % 2 == 0:
        mod *= 2
        if mod >= 10:
            mod_mod = mod % 10
            mod //= 10
            checksum = checksum + mod_mod + mod
        else:
            checksum += mod

    # For non-every other digits, starting with the number's second-to-last digit
    else:
        checksum += mod

    # Record the first 2 digits of the input number
    if 10 <= number < 100:
        first2dg = number

if checksum % 10 == 0:
    if n == 15 and (first2dg == 34 or first2dg == 37):
        print("AMEX")

    elif n == 16 and (51 <= first2dg <= 55):
        print("MASTERCARD")

    elif (n == 13 or n == 16) and (40 <= first2dg <= 49):
        print("VISA")

    else:
        print("INVALID")

else:
    print("INVALID")
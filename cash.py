# Problem Set 6, Sentimental-Cash, Python, 13 July 2019

from cs50 import get_float

# Get changed owed in dollars from user
while True:
    dollars = get_float("Changed owed: ")
    if dollars >= 0:
        break

# Convert dollars to cents
cents = round(dollars * 100)

coins = cents // 25
cents %= 25

coins = coins + cents // 10
cents %= 10

coins = coins + cents // 5
cents %= 5

print(coins + cents)
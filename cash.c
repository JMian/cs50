#include <stdio.h> 
#include <cs50.h>
#include <math.h>

float get_positive_float(string prompt);
int quarters, dimes, nickels, pennies;

int main(void)
{
    float dollars = get_positive_float("Change owed: ");
    int cents = round(dollars * 100);
    for (quarters = 0; cents >= 25; quarters++)
    {
        cents -= 25; 
    }
    for (dimes = 0; cents >= 10; dimes++)
    {
        cents -= 10;
    }
    for (nickels = 0; cents >= 5; nickels++)
    {
        cents -= 5;
    }
    pennies = cents;
    printf("%i\n", quarters + dimes + nickels + cents);  
}


float get_positive_float(string prompt)
{
    float n;
    do
    {
      n = get_float("%s", prompt); 
    }
    while (n < 0);
    return n;
}

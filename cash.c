#include <stdio.h> 
#include <cs50.h>
#include <math.h>

int quarters, dimes, nickels, pennies;

int main(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
   
    int cents = round(dollars * 100);
    
    while (cents >= 25)
    {
        quarters++;
        cents -= 25;
    }
    while (cents >= 10)
    {
        dimes++;
        cents -= 10;
    }
    while (cents >=5)
    {
        nickels++;
        cents -= 5;
    }
    pennies = cents;
    printf("%i\n", quarters + dimes + nickels + cents);  
}

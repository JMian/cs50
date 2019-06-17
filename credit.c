#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    long number = get_long("Number: ");
    
    int digit = 0;
    int mod;
    int mod_mod;
    int checksum = 0;
    int first2dg;
    while (number >= 1)
    {
        mod = number % 10; 
        number /= 10;
        digit++;
        // For every other digit
        if (digit % 2 == 0)
        {            
            mod *= 2;
            // If the every other digit *2 >=10
            if (mod >= 10)
            {
                mod_mod = mod % 10;
                mod = mod / 10;
                checksum = checksum + mod_mod + mod;
            }
            else
            {
                checksum += mod;
            }
        }
        // For non-every-other-digit
        else 
        {
            checksum += mod;
        }
        // Check the first two digits of the number
        if (10 <= number && number < 100)
        {
            first2dg = number;
        }
    }
    if (checksum % 10 == 0)
    {
        if (digit == 15 && (first2dg == 34 || first2dg == 37))
        {
            printf("AMEX\n");
        }
        else if (digit == 16 && 51 <= first2dg && first2dg <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if ((digit == 13 || digit == 16) && 40 <= first2dg && first2dg<= 49)
        {
            printf("VISA\n");
        }
        else 
        {
            printf("INVALID\n");
        }
    }
    else 
    {
        printf("INVALID\n");
    }
}
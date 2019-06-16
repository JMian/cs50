// Problem Set 1, Mario less comfortable, 17 June 2019

#include <cs50.h>
#include <stdio.h>

int get_positive_int(string prompt);

int main(void)
{
    int height = get_positive_int("Height: ");
    for (int i = 0; i < height; i++)
    {
        // Push hashes to the right
        for (int j = 1; j < height - i ; j++)
        {
            printf(" ");
        }
            for (int k = 0; k <= i; k++)
            {
                printf("#");
            }
        printf("\n");
    }     
}

// Prompt user for positive integer
int get_positive_int(string prompt)
{
    int n;
    do
    {
        n = get_int("%s", prompt);
    }
    while (n < 1 || n > 8);
    return n;
}

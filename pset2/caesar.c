// Problem Set 2, Caesar, 20 June 2019
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool alldigits(string s);
int main(int argc, string argv[])
{
    // Check if the command line has 2 arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }   
    // Check if the second command line argument is all composed of digits
    bool checkalldigits = alldigits(argv[1]);
    if (checkalldigits == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }    
    // If conditions are met, start rotating
    int key = atoi(argv[1]);
    string ptx = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(ptx); i < n; i++)
    {
        // If uppercase letters, or use isupper(ptx[i])
        if (65 <= ptx[i] && ptx[i] <= 90)
        {
            printf("%c", (ptx[i] - 65 + key) % 26 + 65);
        }
        // If lowercase letters, or use islower(ptx[i])
        else if (97 <= ptx[i] && ptx[i] <= 122)
        {
            printf("%c", (ptx[i] - 97 + key) % 26 + 97);
        }
        // If is not alphabet
        else
        {
            printf("%c", ptx[i]);
        }
    }
    printf("\n");
}

// Check if the whole string is composed of digits
bool alldigits(string s)
{
    int n = strlen(s);
    for (int i = 0; i < n; i++)
    {
        int test = isdigit(s[i]);
        if (test == 0)
        {
            return false;
        }
    }
    return true;
}
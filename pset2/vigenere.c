// Problem Set 2, Vigenere, 20 June 2019
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool allalphas(string s);
int shift(char c);
int main(int argc, string argv[])
{
    // Check if the command line has 2 arguments
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }   
    // Check if the second command line argument is all composed of alphabets
    bool checkallalphas = allalphas(argv[1]);
    if (checkallalphas == false)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }    
    // If conditions are met, start rotating
    string ptx = get_string("plaintext:  ");
    printf("ciphertext: ");
    int n = strlen(ptx), nj = strlen(argv[1]);
    for (int i = 0, j = 0; i < n; i++)
    {
        // If run out of the keystring, restart from first char of keyword
        if (j >= nj)
        {
            j = 0;
        }
        int key = shift(argv[1][j]);
        if isupper(ptx[i])
        {
            printf("%c", (ptx[i] - 65 + key) % 26 + 65);
            j++;
        }
        else if islower(ptx[i])
        {
            printf("%c", (ptx[i] - 97 + key) % 26 + 97);
            j++;
        }
        else
        {
            printf("%c", ptx[i]);
        }
    }
    printf("\n"); 
}

// Check if the whole string is composed of alphabets
bool allalphas(string s)
{
    int n = strlen(s);
    for (int i = 0; i < n; i++)
    {
        int test = isalpha(s[i]);
        if (test == 0)
        {
            return false;
        }
    }
    return true;
}

// Convert input character to shift value
int shift(char c)
{
    int shiftvalue;
    if isupper(c)
    {
        shiftvalue = c - 65;
    }
    if islower(c)
    {
        shiftvalue = c - 97;
    }
    return shiftvalue;
}

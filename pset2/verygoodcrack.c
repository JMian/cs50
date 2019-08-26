// Problem Set 2, Crack, 21 June 2019
// This was not my own work
// This version was seen from the internet, just to remind myself and for revision
#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <unistd.h>

int main(int argc, string argv[])
{
    // Check command line argument
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    string hash = argv[1];
    char salt[3] = {hash[0], hash[1], '\0'};
    char pw[6] = "\0\0\0\0\0\0";
    string pwlist = "\0abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int fifth = 0; fifth < 53; fifth++)
    {
        for (int fourth = 0; fourth < 53; fourth++)
        {
            for (int third = 0; third < 53; third++)
            {
                for (int second = 0; second < 53; second++)
                {
                    for (int first = 1; first < 53; first++)
                    {
                        pw[0] = pwlist[first];
                        pw[1] = pwlist[second];
                        pw[2] = pwlist[third];
                        pw[3] = pwlist[fourth];
                        pw[4] = pwlist[fifth];
                        if (strcmp(crypt(pw, salt), hash) == 0)
                        {
                            printf("%s\n", pw);
                            return 0;
                        }
                    }
                     
                }
            }
        }
    }
    printf("Password cannot be cracked at this time\n");
}

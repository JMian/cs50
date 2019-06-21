// Problem Set 2, Crack, 21 June 2019
// Have to review and rewrite code
#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <ctype.h>

string getsalt(string s);
bool checkhash(string pw2, string salt, string myhash, string hash);
int main(int argc, string argv[])
{
    // Check command line argument
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    string hash = argv[1];
    string salt = getsalt(hash); 
    string myhash = "";
    int cmp = 1;
    char pw2[6];
    pw2[5] = '\0';
    for (int i = 0; i < 58; i++)
    {
        pw2[1] = '\0', pw2[2] = '\0', pw2[3] = '\0', pw2[4] = '\0';
        pw2[0] = 65 + i;
        bool check = checkhash(pw2, salt, myhash, hash);
        if (check == true)
        {
            return 0;
        }     
        for (int j = 0; j < 58; j++)
        {
            pw2[2] = '\0', pw2[3] = '\0', pw2[4] = '\0';
            pw2[1] = 65 + j;
            check = checkhash(pw2, salt, myhash, hash);
            if (check == true)
            {
                return 0;
            }
            for (int k = 0; k < 58; k++)
            {
                pw2[3] = '\0', pw2[4] = '\0';
                pw2[2] = 65 + k;
                check = checkhash(pw2, salt, myhash, hash);
                if (check == true)
                {
                    return 0;
                }                
                for (int l = 0; l < 58; l++)
                {
                    pw2[4] = '\0';
                    pw2[3] = 65 + l;
                    check = checkhash(pw2, salt, myhash, hash);
                    if (check == true)
                    {
                        return 0;
                    }
                    for (int m = 0; m < 58; m++)
                    {               
                        pw2[4] = 65 + m;
                        check = checkhash(pw2, salt, myhash, hash);
                        if (check == true)
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }        
}


// Get the first two characters(salt) from the input hash
string getsalt(string s)
{
    // Why static?
    static char gs[2] = "";
    for (int i = 0; i < 2; i++)
    {
        gs[i] = s[i];
    }
    return gs;
}

// Check if the current hash resulted is same as the input hash 
bool checkhash(string pw2, string salt, string myhash, string hash)
{

    myhash = crypt(pw2, salt); 
    int cmp = strcmp(myhash, hash);
    if (cmp == 0)
    {       
        printf("%s\n", pw2);
        return true;
    }
    else
    {
        return false;
    }

}

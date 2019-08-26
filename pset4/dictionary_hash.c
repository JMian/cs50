// Problem set 4, Speller, Hashtable, 1 July 2019

// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

unsigned int counter_size = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

bool loaded = false;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char wordcurrent[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", wordcurrent) != EOF)
    {
        // TODO
        // Find which linked list the word belongs to 
        unsigned int firstchar = hash(wordcurrent);
        
        // Allocate node's memory for the new word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        
        // Copy the new word into the word's section of the new node crated
        strcpy(new_node->word, wordcurrent);
        
        // See if there is anything hashtable[i] is pointing to
        new_node->next = (hashtable[firstchar] == NULL) ? NULL : hashtable[firstchar];
        
        // Re-point hashtable[i] from the original node to the new inserted node
        hashtable[firstchar] = new_node;
        
        counter_size++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (loaded)
    {
        return counter_size;
    }
    else
    {
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // A new pointer pointing to the first node of the linked list the word belongs to
    node *checker = hashtable[hash(word)];
    
    // Compare the current word with words from nodes down the respective linked list
    while (checker != NULL)
    {
        if (strcasecmp(word, checker->word) == 0)
        {
            return true;
        }
        checker = checker->next;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // Iterate over all the linked list (all the array's elements)
    for (int i = 0; i < N; i++)
    {
        // A new pointer pointing to the first node of the linked list
        node *unloader = hashtable[i];
        // Iterating and freeing nodes down the linked list
        while (unloader != NULL)
        {
            node *temp = unloader;
            unloader = unloader->next;
            free(temp);
        }
        // Check if successfully iterating over and finishing i == N - 1 linked lists
        if (i == N - 1)
        {
            return true;
        }
    }
    return false;
}

// Problem set 4, Speller, Tries, 3 July 2019
// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

unsigned int counter_size = 0;
bool loaded = false;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    // If use malloc instead, has to use for loop to initialize every root->children[N] to NULL
    root = calloc(1, sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    
    // Or use memset() to initialize both root->children[] = NULL and root->is_word false
    root->is_word = false;

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        // Get the number of characters composing the particular word
        int n = strlen(word);
        
        // Create a mobile/movable node
        node *root2 = root;
        
        // Iterate over the characters of the word and if necessary, create respective children nodes
        for (int j = 0; j < n; j++)
        {
            // Convert the character ASCII to array element's number
            int i = (word[j] == '\'') ? 26 : tolower(word[j]) - 'a';
            
            // If the current character is pointing to null i.e. this character hasn't been used before 
            if (root2->children[i] == NULL)
            {
                // Allocate memory for a new node containing an array
                node *temp = calloc(1, sizeof(node));
                temp->is_word = false;
                
                // The pointer from the current character is no longer pointing to null,
                // it is now pointing towards the new node/array created
                root2->children[i] = temp;
            }
            
            // Change the starting point of the current pointer from pointing towards the current character 
            // to pointing to the next array from this character
            root2 = root2->children[i];
        }
        
        // The characters of the word have been iterated successfully, the end of the word is reached
        root2->is_word = true;
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
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int n = strlen(word);
    
    node *root2 = root;
    
    for (int j = 0; j < n; j++)
    {
        int i = (word[j] == '\'') ? 26 : tolower(word[j]) - 'a';
        
        root2 = root2->children[i];
        
        if (root2 == NULL)
        {
           return false;
        }
    }
    if (root2->is_word)
    {
        return true;
    }
    
    return false;
}


void clear(node* root2)
{
    for (int i = 0; i < N; i++)
    {
        if (root2->children[i] != NULL)
        {
            clear(root2->children[i]);
        }
    }
 
    free(root2);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    clear(root);
    return true;
}

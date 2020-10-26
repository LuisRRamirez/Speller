// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// global variable for tracking dictionary size
unsigned int word_count = 0;

// global boolean for tracking load/unload dictionary operations
bool loaded = false;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 60;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hashvalue = hash(word);
    node *n = table[hashvalue];
    
    while ( n != NULL )
    {
        if ( strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // reference: https://www.reddit.com/r/cs50/comments/eo4zro/good_hash_function_for_speller/
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);
    while (*word != 0)
    {
        hash = (( hash << 5 ) + hash) + c; 
        c = *word++;
        c = tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // This opens the dictionary file
    FILE *file = fopen(dictionary , "r");
    // buffer for word to be used when scanning 
    char word[LENGTH+1];
    if (file == NULL)
    {
        unload();
        return false;
    }
    // Reads the string one at a time
    while (fscanf(file , "%s", word) != EOF)
    {
        // creating a new node from our node "function" and we're assigning it memory
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            unload();
            return false;
        }
        //creating a hash
        int hashvalue = hash(word);
        // copying n and pointing it to word 
        strcpy(n->word, word);
        // making sure the next the btm value is NULL
        n->next=NULL;

        //when a new word comes in, it pushes the first word to the second word, and the new word becomes the first word with pointers
        n->next = table[hashvalue];
        table[hashvalue] = n;
        
        
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // this checks every value of N 
    for (int i = 0; i < N; i++)
    {
        //making the struct node point to the table 
        node *n = table[i];
        if (n != NULL)
        {
            
            word_count++;
            // once i'm done with this word, i move on to the next one 
            n=n->next;
        }
    }
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        node *cursor = head;

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
            temp=cursor;
        }

        
    }
    return true;
}

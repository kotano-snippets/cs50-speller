// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 16777216; // 2^24
// const unsigned int N = 64500; // 2^24

unsigned int words_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int len = strlen(word);
    char word_lower[len + 1];
    for (int i = 0; word[i] != '\0'; i++)
    {
        word_lower[i] = tolower(word[i]);
    }
    word_lower[len] = '\0';
    unsigned int index = hash(word_lower);

    node *n = table[index];
    if (n == NULL)
    {
        return false;
    }
    for (; n != NULL; n = n->next)
    {
        if (strcmp(word_lower, n->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    const int p = 31;
    // const int N = 1e9 + 9;
    long long hash_value = 0;
    int p_pow = 1;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        if (isalpha(word[i]))
        {
            hash_value = (hash_value + (word[i] - 'a' + 1) * p_pow) % N;
            p_pow = (p_pow * p) % N;
        }
    }
    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char word[LENGTH + 1];
    node *n = NULL;
    unsigned int index;
    // Open dictionary file
    FILE *fdic = fopen(dictionary, "r");
    if (fdic == NULL)
    {
        printf("Unable to open dictionary file\n");
        return false;
    }
    while (fscanf(fdic, "%s", word) != EOF)
    {
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Unable to allocate memory for %s\n", word);
            return false;
        }
        strcpy(n->word, word);
        index = hash(word);
        // Set new node to point at existing node
        n->next = table[index];
        // Set root node to point at new node
        table[index] = n;
        words_count += 1;
    }
    fclose(fdic);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_count;
}

bool free_node(node *n)
{
    // If we found last node, free it
    if (n == NULL)
    {
        return true;
    }
    else if (n->next == NULL)
    {
        free(n);
        return true;
    }
    // Free child nodes and check everything is ok
    else if (!free_node(n->next))
    {
        return false;
    }
    // Free current node
    free(n);
    return true;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *n;
    for (unsigned int i = 0; i <= N; i++)
    {
        if (free_node(table[i]) != true)
        {
            return false;
        }
    }
    return true;
}

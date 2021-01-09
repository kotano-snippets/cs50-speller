// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
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

unsigned int words_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char word_lower[LENGTH];
    for (int i = 0; word[i]; i++)
    {
        word_lower[i] = tolower(word[i]);
        printf("%c\n", word[i]);
    }
    unsigned int index = hash(word_lower);

    node *n = table[index];

    for (; n->next != NULL; n = n->next)
    {
        if (strcmp(n->word, word_lower) == 0) {
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

    for (int i; i < len; i++)
    {
        hash_value = (hash_value + (word[i] - 'a' + 1) * p_pow) % N;
        p_pow = (p_pow * p) % N;
    }
    return hash_value;
}
// return (unsigned int)tolower(word[0]) - 97;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char word[LENGTH];
    node *n = NULL;
    unsigned int index;
    // Open dictionary file
    FILE *fdic = fopen(dictionary, "r");
    if (fdic == NULL)
    {
        printf("Unable to open dictionary file\n");
        return false;
    }
    while (fscanf(fdic, "%s", word))
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
        // TODO: Check
        printf("%ui", index);
    }

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_count;
}

bool free_node(node *n)
{
    printf("%s\n", n->word);
    // If we found last node, free it
    if (n->next == NULL)
    {
        free(n);
        return true;
    }
    // Free child nodes and check everything is ok
    if (!free_node(n->next))
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
    for (unsigned int i = 0; i < N; i++)
    {
        printf("INDEX: %ui", i);
        free_node(table[i]);
        // for (n = table[i]; n->next != NULL; n = n->next)
        // {

        // }
    }
    words_count = 0;
    return false;
}

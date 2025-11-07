// dictionary.c
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Counter for number of words loaded
static unsigned int word_count = 0;

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381UL;
    for (const unsigned char *p = (const unsigned char *) word; *p; p++)
    {
        unsigned char c = (unsigned char)tolower(*p);
        hash = ((hash << 5) + hash) + c;
    }
    return (unsigned int)(hash % N);
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    if (word == NULL)
        return false;

    unsigned int index = hash(word);

    node *cursor = table[index];
    while (cursor != NULL)
    {
        const char *a = cursor -> word;
        const char *b = word;
        for (; *a != '\0' && *b != '\0'; a++, b++) // There's no need to initialize variables there
        {
            if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
                break;
        }
        if (*a == '\0' && *b == '\0')
            return true;

        cursor = cursor -> next;
    }
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (unsigned int i = 0; i < N; i++)
        table[i] = NULL;
    word_count = 0;

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
        return false;

    char buffer[LENGTH + 1];
    while (fscanf(file, "%45s", buffer) == 1)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            unload();
            return false;
        }
        strcpy(new_node -> word, buffer);
        unsigned int index = hash(buffer);
        new_node -> next = table[index];
        table[index] = new_node;
        word_count++;
    }

    fclose(file);
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor -> next;
            free(tmp);
        }
        table[i] = NULL;
    }
    word_count = 0;
    return true;
}

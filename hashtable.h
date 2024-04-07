/**
 * \author Nathan Winslow
 *
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

static const uint8_t HASH_TABLE_SIZE = 255u;

typedef struct hash_item_t {
  char *key;
  char *value;
} hash_item_t;

/**
 * \brief A linked list of items is created to avoid collisions when inserting
 * elements into the hash table
 */
typedef struct linked_list_t {
  hash_item_t *item;
  struct linked_list_t *next;
} linked_list_t;

/**
 * \brief Organized by { Key: Value } pairs.
 *
 */
typedef struct hash_table_t {
  hash_item_t **items;
  linked_list_t **buckets;
  int table_size;
  int num_elements;
} hash_table_t;

/**
 * \brief allocate memory for a linked list pointer
 *
 */
linked_list_t *linked_list_allocate();

/**
 * \brief insert an item into the linked list.
 *
 * \return pointer to the head
 */
linked_list_t *linked_list_insert(linked_list_t *list, hash_item_t *item);

/**
 * \brief removes the head from the linked list.
 *
 * \return the removed item from the head of the linked list.
 *
 */
hash_item_t *linked_list_pop(linked_list_t *list);

/**
 * \brief free the linked list from memory
 *
 */
void linked_list_free(linked_list_t *list);

/**
 * \brief When there is a collision in the hash table, an overflow bucket is
 * created to link items with the same hash value.
 *
 * buckets are an array of linked lists.
 *
 */
linked_list_t **hash_table_create_overflow_buckets(hash_table_t *table);

/**
 * \brief Free all of the overflow buckets.
 */
void hash_table_free_buckets(hash_table_t *table);

/**
 * \brief creates a hash item object
 *
 * \param key 2 character string
 *
 * \param int value the output of the hash function
 *
 * \return hash_item* reference to hash_item
 *
 */
hash_item_t *create_item(char *key, char *value);

/**
 * \brief generates a hash value used to look up
 * function pointers during parsing.
 *
 * \param str char array pointer to key
 *
 * \return int hash_value
 *
 */
int hash_function(char *str);

/**
 * \brief Constructor for the hash table
 *
 * \return pointer to the root of the table
 */
hash_table_t *hash_table_create(int size);

/**
 * \brief Helper for freeing items out of the hash table
 *
 * \param item reference to item to free
 *
 */
void hash_table_free_item(hash_item_t *item);

/**
 * \brief Helper for freeing up the whole hash table
 *
 * \param reference to the hash table
 *
 */
void hash_table_free_table(hash_table_t *table);

/**
 * \brief Inserts a value into the hash table
 *
 */
void hash_table_insert(hash_table_t *table, char *key, char *value);

/**
 * \brief Remove an entry from the hash table
 *
 * \param key: Key for the entry in the table.
 */
void hash_table_delete(hash_table_t *table, char *key);

/**
 * \brief Helper function to print the contents of the hash table
 */
void hash_table_print(hash_table_t *table);

/**
 * \brief When a collision is detected, this function places the item
 * into a linked list of items who share a common index in the hash table
 */
void hash_table_handle_collision(hash_table_t *table, int index,
                                 hash_item_t *item);

/**
 * \brief Search the hash table for a value associated with the key.
 *
 * \param table
 * \param key cstring
 *
 * \return value at key, else NULL
 */
char *hash_table_search(hash_table_t *table, char *key);

/**
 * \brief Helper function to display the contents of a search
 */
void hash_table_print_search(hash_table_t *table, char *key);

#endif /* END HASHTABLE_H */

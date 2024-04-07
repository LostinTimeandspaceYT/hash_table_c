/** @file hashtable.c
 *
 * @brief This module is the hash table implementation.
 *
 * @author Nathan Winslow, 2024
 *
 * @par
 * credit to Digital Ocean author, Vijaykrishna Ram for the basis of this data
 * structure.
 *
 * The intention with this module is to expand on the initial work and create a
 * generic `hash_table_t` .
 *
 * source:
 * https://www.digitalocean.com/community/tutorials/hash-table-in-c-plus-plus
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

static const uint8_t HASH_TABLE_SIZE = 255u;

int hash_function(char *str) {
  int hash_val = 0;

  int i;
  for (i = 0; str[i]; ++i) {
    hash_val += (int)str[i];
    hash_val = hash_val << 1;
  }

  /*   TODO: do some other math here */

  return hash_val % HASH_TABLE_SIZE;
}

hash_item_t *create_item(char *key, char *value) {
  hash_item_t *item = (hash_item_t *)malloc(sizeof(hash_item_t));
  item->key = (char *)malloc(strlen(key) + 1);
  item->value = ((char *)malloc(strlen(value) + 1));
  strcpy(item->key, key);
  strcpy(item->value, value);
  return item;
}

hash_table_t *hash_table_create(int size) {
  hash_table_t *table = (hash_table_t *)malloc(sizeof(hash_table_t));
  table->table_size = size;
  table->num_elements = 0;
  table->items = (hash_item_t **)calloc(table->table_size, sizeof(hash_item_t));

  for (int i = 0; i < table->table_size; ++i) {
    table->items[i] = NULL;
  }

  table->buckets = hash_table_create_overflow_buckets(table);
  return table;
}

void hash_table_free_item(hash_item_t *item) {
  free(item->key);
  free(item->value);
  free(item);
}

void hash_table_free_table(hash_table_t *table) {
  int i;
  for (i = 0; i < table->table_size; ++i) {
    if (table->items[i] != NULL) {
      hash_table_free_item(table->items[i]);
    }
  }

  hash_table_free_buckets(table);
  free(table->items);
  free(table);
}

void hash_table_print(hash_table_t *table) {
  printf("|------------------------| HASH TABLE |------------------------|\n");
  int i;
  for (i = 0; i < table->table_size; ++i) {
    if (table->items[i]) {
      printf("Index:%d\tKey:%s\tValue:%s\n", i, table->items[i]->key,
             table->items[i]->value);
    }
  }
  printf("|------------------------| !END TABLE |------------------------|\n");
}

void hash_table_insert(hash_table_t *table, char *key, char *value) {
  hash_item_t *item = create_item(key, value);
  int index = hash_function(key);
  hash_item_t *current_item = table->items[index];

  if (current_item == NULL) {
    if (table->num_elements == table->table_size) {
      printf("Insert Error: Hash Table is full\n");
      hash_table_free_item(item);
      return;
    }

    // Insert directly.
    table->items[index] = item;
    table->num_elements++;
  } else {
    if (strcmp(current_item->key, key) == 0) {
      table->items[index]->value = value;
    } else {
      hash_table_handle_collision(table, index, item);
      return;
    }
  }
}

void hash_table_delete(hash_table_t *table, char *key) {
  int index = hash_function(key);
  hash_item_t *item = table->items[index];
  linked_list_t *head = table->buckets[index];
  if (item == NULL) {
    return;
  }

  if (head == NULL && strcmp(item->key, key) == 0) {
    table->items[index] = NULL;
    hash_table_free_item(item);
    table->num_elements--;
    return;

  } else if (head != NULL) {
    if (strcmp(item->key, key)) {
      hash_table_free_item(item);
      linked_list_t *node = head;
      head = head->next;
      node->next = NULL;
      table->items[index] = create_item(node->item->key, node->item->value);
      linked_list_free(node);
      table->buckets[index] = head;
      return;
    }

    linked_list_t *curr = head;
    linked_list_t *prev = NULL;

    while (curr) {
      if (strcmp(item->key, key)) {
        if (prev == NULL) {
          linked_list_free(head);
          table->buckets[index] = NULL;
          return;
        }
        prev->next = curr->next;
        curr->next = NULL;
        linked_list_free(curr);
        table->buckets[index] = head;
        return;
      }
      curr = curr->next;
      prev = curr;
    }
  }
}

void hash_table_handle_collision(hash_table_t *table, int index,
                                 hash_item_t *item) {
  linked_list_t *head = table->buckets[index];

  if (head != NULL) {
    table->buckets[index] = linked_list_insert(head, item);
    return;
  }
  head = linked_list_allocate();
  head->item = item;
  table->buckets[index] = head;
  return;
}

char *hash_table_search(hash_table_t *table, char *key) {
  int index = hash_function(key);
  hash_item_t *item = table->items[index];
  linked_list_t *head = table->buckets[index];

  if (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      return item->value;
    }
    if (head == NULL) {
      return NULL;
    }
    item = head->item;
    head = head->next;
  }
  return NULL;
}

void hash_table_print_search(hash_table_t *table, char *key) {
  char *val = hash_table_search(table, key);

  if (val == NULL) {
    printf("Key %s does not exist \n", key);
  } else {
    printf("Key:%s\tValue:%s\n", key, val);
  }
}

linked_list_t *linked_list_allocate() {
  linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
  return list;
}

linked_list_t *linked_list_insert(linked_list_t *list, hash_item_t *item) {
  if (!list) {
    linked_list_t *head = linked_list_allocate();

    head->item = item;
    head->next = NULL;
    list = head;
    return list;
  } else if (list->next == NULL) {
    linked_list_t *node = linked_list_allocate();
    node->item = item;
    node->next = NULL;
    list->next = node;
    return list;
  }

  linked_list_t *temp = list;

  /* Find the end of the list */
  while (temp->next->next) {
    // WARN:TEST ME!!!!
    temp = temp->next;
  }

  linked_list_t *node = linked_list_allocate();
  node->item = item;
  node->next = NULL;
  temp->next = node;
  return list;
}

hash_item_t *linked_list_pop(linked_list_t *list) {
  if (!list || !(list->next)) {
    return NULL;
  }
  linked_list_t *node = list->next;
  linked_list_t *tmp = list;
  tmp->next = NULL;
  list = node;

  hash_item_t *item = NULL;
  memcpy(tmp->item, item, sizeof(hash_item_t));

  free(tmp->item->key);
  free(tmp->item->value);
  free(tmp->item);
  free(tmp);

  return item;
}

void linked_list_free(linked_list_t *list) {
  linked_list_t *tmp = list;

  while (list) {
    tmp = list;
    list = list->next;
    free(tmp->item->value);
    free(tmp->item->key);
    free(tmp->item);
    free(tmp);
  }
}

linked_list_t **hash_table_create_overflow_buckets(hash_table_t *table) {
  linked_list_t **buckets =
      (linked_list_t **)calloc(table->table_size, sizeof(linked_list_t *));

  for (int i = 0; i < table->table_size; ++i) {
    buckets[i] = NULL;
  }

  return buckets;
}

void hash_table_free_buckets(hash_table_t *table) {
  linked_list_t **buckets = table->buckets;

  for (int i = 0; i < table->table_size; ++i) {
    linked_list_free(buckets[i]);
  }
  free(buckets);
}

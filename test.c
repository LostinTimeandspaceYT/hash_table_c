/** @file test.c
 *
 * @brief The purpose of this module is to test the functionality of the hash
 * table.
 *
 * @author Nathan Winslow
 *
 * @par
 * Source:
 * https://www.digitalocean.com/community/tutorials/hash-table-in-c-plus-plus
 *
 */

#include "hashtable.h"
#include <stdint.h>

static const uint8_t MY_TABLE_SIZE = 100u;

int main() {
  hash_table_t *table = hash_table_create(MY_TABLE_SIZE);

  hash_table_insert(table, (char *)"1", (char *)"First Address");
  hash_table_insert(table, (char *)"2", (char *)"Second Address");
  hash_table_insert(table, (char *)"3", (char *)"Third Address");
  hash_table_insert(table, (char *)"4", (char *)"Fourth Address");
  hash_table_insert(table, (char *)"Hel", (char *)"Fifth Address");
  hash_table_insert(table, (char *)"Cau", (char *)"Sixth Address");
  hash_table_insert(table, (char *)"7", (char *)"Seventh Address");
  hash_table_insert(table, (char *)"8", (char *)"Eighth Address");
  hash_table_insert(table, (char *)"9", (char *)"Ninth Address");
  hash_table_insert(table, (char *)"10", (char *)"Tenth Address");

  hash_table_print(table);
  hash_table_print_search(table, (char *)"1");
  hash_table_print_search(table, (char *)"2");
  hash_table_print_search(table, (char *)"3");

  hash_table_print_search(table, (char *)"Hel");

  // This should cause a collision
  hash_table_print_search(table, (char *)"Cau");

  hash_table_print(table);
  hash_table_delete(table, (char *)"1");
  hash_table_delete(table, (char *)"Cau");

  hash_table_print(table);
  return 0;
}

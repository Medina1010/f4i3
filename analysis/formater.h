#ifndef FORMATER_H
#define FORMATER_H

#include <stdlib.h>

typedef struct {
        size_t count;
        char keys[128][128];
        char values[128][128];
} table_t;                                              

void table_append(table_t *tbl, char *key, char *value);
int table_search(table_t *tbl, char *key);
void table_define(table_t *tbl, char *key, char *value);
void table_append_float(table_t *tbl, char *key, float value, int decimals);
void formater(char *src, char *dst, table_t *tbl);
void formater_file (char *src_path, char *dst_path, table_t *tbl);

#endif

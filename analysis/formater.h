#ifndef FORMATER_H
#define FORMATER_H

typedef struct {
        size_t count;
        char keys[128][16];
        char values[128][16];
} table_t;                                              

void table_append(table_t *tbl, char *key, char *value);
void formater(char *src, char *dst, table_t *tbl);

#endif

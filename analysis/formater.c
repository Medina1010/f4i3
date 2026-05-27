#include <string.h>
#include "formater.h"
#include <stdio.h>
#include <stdlib.h>

void table_append(table_t *tbl, char *key, char *value) {
	strcpy(tbl->keys[tbl->count], key);
	strcpy(tbl->values[tbl->count], value);
	tbl->count++;
}

int table_search(table_t *tbl, char *key) {
	for (size_t i = 0; i < tbl->count; i++)
		if (!strcmp(tbl->keys[i], key))
			return i;
	return -1;
}

void table_define(table_t *tbl, char *key, char *value) {
	int index = table_search(tbl, key);
	if (index != -1)
		strcpy(tbl->values[index], value);
	else
		table_append(tbl, key, value);
}

void table_append_float(table_t *tbl, char *key, float value, int decimals) {
	strcpy(tbl->keys[tbl->count], key);
	sprintf(tbl->values[tbl->count], "%.*f", decimals, value);
	tbl->count++;
}
	
void table_define_float(table_t *tbl, char *key, float value, int decimals) {
	int index = table_search(tbl, key);
	if (index != -1)
		sprintf(tbl->values[index], "%.*f", decimals, value);
	else
		table_append_float(tbl, key, value, decimals);
}

void formater(char *src, char *dst, table_t *tbl) {
	size_t v_beg = 0;
	size_t j = 0;
	for (size_t i = 0; i <= strlen(src) ; i++) {
		if (src[i] == '$' && src[i+1] == '(') {
			i += 2;
			v_beg = i;
		}
		if (v_beg != 0) {
			if (src[i] == ')') {
				char *text = malloc(i - v_beg);
				strncpy(text, &src[v_beg], i - v_beg);
				text[i - v_beg] = 0;
				int k = -1;
				if ( (k = table_search(tbl, text)) != -1) {
						strcpy(&dst[j], tbl->values[k]);
						j += strlen(tbl->values[k]);
				}
				free(text);
				v_beg = 0;
			}
		} else {
			dst[j++] = src[i];
		}
	}

}

typedef struct {
	size_t count, cap;
	char *data;
} chars;

void chars_from_file(chars *str, char *path) {
	FILE *file = fopen(path, "r");
	while(!feof(file)) {
		if (str->count >= str->cap) {
			str->cap = str->cap == 0 ? 1 : str->cap * 2;
			str->data = realloc(str->data, str->cap);
		}
		int ch = fgetc(file);
		str->data[str->count++] = ch == EOF ? 0 : ch ;
	}
	fclose(file);
}

void str_to_file(char *str, char *path) {
	FILE *file = fopen(path, "w");
	for (size_t i = 0; i < strlen(str); i++)
		fputc(str[i], file);
	fclose(file);
}

void formater_file (char *src_path, char *dst_path, table_t *tbl) {
	chars src = {0};
	chars_from_file (&src, src_path);
	char *dst = malloc(src.count * 2);
	formater(src.data, dst, tbl);
	str_to_file(dst, dst_path);
	free(src.data);
	free(dst);
}

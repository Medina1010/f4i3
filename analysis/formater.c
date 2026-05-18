#include <string.h>
#include "formater.h"
#include <stdio.h>
#include <stdlib.h>

void table_append(table_t *tbl, char *key, char *value) {
	strcpy(tbl->keys[tbl->count], key);
	strcpy(tbl->values[tbl->count], value);
	tbl->count++;
}

void formater(char *src, char *dst, table_t *tbl) {
	size_t v_beg = 0;
	size_t j = 0;
	for (size_t i = 0; i < strlen(src); i++) {
		if (src[i] == '$' && src[i+1] == '(') {
			v_beg = i + 2;
			i += 2;
		}
		if (v_beg != 0) {
			if (src[i] == ')') {
				for( size_t k = 0; k < tbl->count; k++) {
					if (!strncmp(&src[v_beg], tbl->keys[k], i - v_beg)) {
						strcpy(&dst[j], tbl->values[k]);
						j += strlen(tbl->values[k]);
					}
				}
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
			str->cap = str->cap ? str->cap * 2 : 1;
			str->data = realloc(str->data, str->cap);
		}
		str->data[str->count++] = fgetc(file);
	}
	if (str->count >= str->cap) {
		str->cap = str->cap ? str->cap * 2 : 1;
		str->data = realloc(str->data, str->cap);
	}
	str->data[str->count++] = '\0';

	fclose(file);
}

void str_to_file(char *str, char *path) {
	FILE *file = fopen(path, "w");
	fprintf(file, "%s", str);
	fclose(file);
}

void formater_file (char *src_path, char *dst_path, table_t *tbl) {
	chars src;
	chars_from_file (&src, src_path);
	char *dst = malloc(strlen(src.data));
	formater(src.data, dst, tbl);
	str_to_file(dst, dst_path);
	free(src.data);
	free(dst);
}

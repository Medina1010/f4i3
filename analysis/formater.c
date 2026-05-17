#include <string.h>
#include <formater.h>
#include <stdio.h>

typedef struct {
	size_t count;
	char keys[128][16];
	char values[128][16];
} table_t;

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

void formater_file (char *src_path, char *dst_path) {
	chars src, dst;
	chars_from_file (&src, src_path);
	chars_from_file (&dst, dst_path);
	formater(src->data, dst->data, tbl);
	free(src->data);
	free(dst->data);
}



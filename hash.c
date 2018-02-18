# include "arc.h"
# include <string.h>
typedef struct entry {
	struct entry *next;
	char const *key;
	mdl_u64_t no;
} *entryp;

entryp table[0xff];

mdl_u8_t sum(char const *__key, mdl_u8_t __len) {
	mdl_u8_t ret = 0xff;
	char const *p = __key;
	char const *end = p+__len;
	while(p != end)
		ret = ret^*(p++);
	return ret;
}

# include <malloc.h>
mdl_u64_t hash_put(char const *__key, mdl_u64_t __no) {
	mdl_u8_t l = strlen(__key);
	entryp *p = table+sum(__key, l);

	entryp r = (entryp)malloc(sizeof(struct entry));
	r->next = NULL;
	if (!*p)
		*p = r;
	else {
		r->next = *p;
		*p = r;
	}

	(*p)->key = strndup(__key, l);
	(*p)->no = __no;
	return __no;
}

# include <stdio.h>
mdl_u64_t hash_get(char const *__key) {
	mdl_u8_t l = strlen(__key);
	entryp *p = table+sum(__key, l);
	
	entryp cur = *p;
	while(cur != NULL) {
		if (!strcmp(cur->key, __key)) return cur->no;
		cur = cur->next;
	}
	fprintf(stderr, "lookup failed.\n");
	return -1;
}

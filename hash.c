# include "arc.h"
# include <string.h>
# include "lat/lat.h"
extern struct lat la;
typedef struct entry {
	struct entry *next;
	char const *key;
	mdl_u64_t no;
} *entryp;

entryp table[0xff];

mdl_u64_t sum(char const *__key, mdl_u8_t __len) {
	mdl_u64_t ret = 0xffffffffffffffff;
	char const *p = __key;
	char const *end = p+__len;
	while(p != end) {
		ret = ret^*(p++);
		ret = ret>>56|ret<<8;
	}
	return ret;
}

# include <malloc.h>
mdl_u64_t hash_put(char const *__key, mdl_u64_t __no) {
	mdl_u8_t l = strlen(__key);
	mdl_u64_t s;
	entryp p = (entryp)lat_get(&la, s = sum(__key, l));


	entryp r = (entryp)malloc(sizeof(struct entry));
	r->next = NULL;
	if (p != NULL)
		r->next = p;

	r->key = strndup(__key, l);
	r->no = __no;

	lat_put(&la, s, r);
	return __no;
}

# include <stdio.h>
mdl_u64_t hash_get(char const *__key) {
	mdl_u8_t l = strlen(__key);
	entryp cur = (entryp)lat_get(&la, sum(__key, l));
	while(cur != NULL) {
		if (!strcmp(cur->key, __key)) return cur->no;
		cur = cur->next;
	}
	fprintf(stderr, "lookup failed.\n");
	return -1;
}

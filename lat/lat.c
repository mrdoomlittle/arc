# define __internal
# include "lat.h"
# include <stdio.h>
# include <malloc.h>
mdl_u64_t used = 0;
void* lat_mal(mdl_uint_t __size) {
	used+=__size;
	void *p = malloc(__size+sizeof(mdl_uint_t));
	*(mdl_uint_t*)p = __size;	
	return (void*)((mdl_u8_t*)p+sizeof(mdl_uint_t));
}

void lat_mfr(void *__p) {
	void *p = (void*)((mdl_u8_t*)__p-sizeof(mdl_uint_t));
	used-=*(mdl_uint_t*)p;
	free(p);
}

podp alloc_pod() {
	podp ret = lat_mal(sizeof(struct pod));
	podp *p = (podp*)(ret->p = lat_mal(0x100*sizeof(podp)));
	while(p != ret->p+0x100)
		*(p++) = NULL;
	return ret;
}

mdl_uint_t mem_usage() {return used;}
void lat_prepare(latp __lat) {
	__lat->p = alloc_pod();
}

void lat_put(latp __lat, mdl_u64_t __key, void *__p) {
	podp *p = __lat->p->p;
	if (!*(p = (p+((__key&0xff)^(__key>>8&0xff))))) {
		*p = alloc_pod();
	}

	if (!*(p = (podp*)(*p)->p+((__key>>16&0xff)^(__key>>24&0xff)))) {
		*p = alloc_pod();
	}

	if (!*(p = (podp*)(*p)->p+((__key>>32&0xff)^(__key>>40&0xff)))) {
		*p = alloc_pod();
	}

	if (!*(p = (podp*)(*p)->p+((__key>>48&0xff)^(__key>>56&0xff)))) {
		*p = (podp)lat_mal(sizeof(struct pod));
		(*p)->p = NULL;
	}

	recordp rec = lat_mal(sizeof(struct record));
	rec->next = (recordp)(*p)->p;
	(*p)->p = (void*)rec;
	rec->key = __key;
	rec->p = __p;
}


void* lat_get(latp __lat, mdl_u64_t __key) {
	podp *p = (podp*)__lat->p->p;
	if (!*(p = (p+((__key&0xff)^(__key>>8&0xff))))) {
		fprintf(stderr, "pod 0 failure.\n");
		return NULL;
	}

	if (!*(p = (podp*)(*p)->p+((__key>>16&0xff)^(__key>>24&0xff)))) {
		fprintf(stderr, "pod 2 failure.\n");
		return NULL;
	}

	if (!*(p = (podp*)(*p)->p+((__key>>32&0xff)^(__key>>40&0xff)))) {
		fprintf(stderr, "pod 4 failure.\n");
		return NULL;
	}
  
	if (!*(p = (podp*)(*p)->p+((__key>>48&0xff)^(__key>>56&0xff)))) {
		fprintf(stderr, "pod 6 failure.\n");
		return NULL;
	}

	recordp rec = (recordp)(*p)->p;
	while(rec != NULL) {
		if (rec->key == __key) return rec->p;
		rec = rec->next;
	}
	
	fprintf(stderr, "not found.\n");
	return NULL;
}

void free_pod(podp __pod) {
	mdl_u8_t static depth = 0;
	if (depth < 3) {
		depth++;
		podp *p = (podp*)__pod->p;
		while(p != (podp*)__pod->p+0x100) {
			if (*p != NULL) {
				free_pod(*p);
				lat_mfr(*p);
			}
			p++;
		}
		depth--;
		if (__pod->p != NULL)
			lat_mfr(__pod->p);
	} else {
		recordp rec = (recordp)__pod->p, bk;
		while(rec != NULL) {
			bk = rec;	
			rec = rec->next;
			lat_mfr(bk);
		}
	}
}

void lat_free(latp __lat) {
	free_pod(__lat->p);
	lat_mfr(__lat->p);
}

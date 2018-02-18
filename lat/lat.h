# ifndef __lat__h
# define __lat__h
# include <mdlint.h>
# ifdef __internal
typedef struct record {
	struct record *next;
	mdl_u64_t key;
	void *p;
} *recordp;

typedef struct pod {
	void *p;
} *podp;

# endif

typedef struct lat {
# ifdef __internal
	podp p;	
# else
	mdl_u8_t pad[8];
# endif
} *latp;

mdl_uint_t mem_usage();
void lat_prepare(latp);
void lat_put(latp, mdl_u64_t, void*);
void* lat_get(latp, mdl_u64_t);
void lat_free(latp);
# endif /*__lat__h*/

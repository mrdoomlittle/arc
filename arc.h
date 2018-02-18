# ifndef __arc__h
# define __arc__h
# include <mdlint.h>
enum {
	_rec_arc,
	_rec_raw
};

typedef struct record {
	mdl_u64_t no;
	mdl_u8_t sort;
	struct record *fd, *bk;
	void *p;
} *recordp;

typedef struct arc {
	recordp *reg, p;
	struct arc *bk;
} *arcp;

recordp arc_lookup(arcp, mdl_u64_t);
void arc_prepare(arcp);
arcp creatarc(arcp, mdl_u64_t);
void delarc(arcp);

recordp creatrec(arcp, mdl_u64_t, void*, mdl_u8_t);
void delrec(arcp, recordp);

void *rec_alloc(mdl_uint_t);
void rec_free(void*);

void arc_free(arcp);
void writerec(recordp, void*, mdl_uint_t, mdl_uint_t);
void readrec(recordp, void*, mdl_uint_t, mdl_uint_t);
# endif /*__arc__h*/

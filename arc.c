# include "arc.h"
# include <malloc.h>
# include <stdio.h>
# include <string.h>
void arc_prepare(arcp __arc) {
	__arc->reg = (recordp*)malloc(0xff*sizeof(recordp));
	recordp *p = __arc->reg;
	recordp *end = p+0xff;
	while(p != end)
		*(p++) = NULL;
}

recordp arc_lookup(arcp __arc, mdl_u64_t __no) {
	recordp cur = *(__arc->reg+(__no&0xff));
	while(cur != NULL) {
		if (cur->no == __no) return cur;
		cur = cur->fd;
	}
	return NULL;
}

void writerec(recordp __rec, void *__buf, mdl_uint_t __off, mdl_uint_t __n) {
	memcpy((mdl_u8_t*)__rec->p+__off, __buf, __n);
}

void readrec(recordp __rec, void *__buf, mdl_uint_t __off, mdl_uint_t __n) {
	memcpy(__buf, (mdl_u8_t*)__rec->p+__off, __n);
}

void *rec_alloc(mdl_uint_t __n) {
	return malloc(__n);
}

void rec_free(void *__p) {
	free(__p);
}

recordp creatrec(arcp __arc, mdl_u64_t __no, void *__p, mdl_u8_t __sort) {
	recordp *p = __arc->reg+(__no&0xff);

	recordp rec = (recordp)malloc(sizeof(struct record));	
	rec->bk = NULL;
	rec->fd = *p;
	if (*p != NULL)
		(*p)->bk = rec;
	*p = rec;
	rec->no = __no;
	rec->p = __p;
	rec->sort = __sort;
	return rec;
}

void delrec(arcp __arc, recordp __rec) {
	recordp *p = __arc->reg+(__rec->no&0xff);
	if (*p == __rec)
		*p = __rec->fd;
	else {
		__rec->bk->fd = __rec->fd;
		__rec->fd->bk = __rec->bk;
	}
	if (__rec->p != NULL)
		rec_free(__rec->p);
}

arcp creatarc(arcp __arc, mdl_u64_t __no) {
	recordp rec = creatrec(__arc, __no, rec_alloc(sizeof(struct arc)), _rec_arc);
	arcp arc = (arcp)rec->p;
	arc_prepare(arc);
	arc->bk = __arc;
	arc->p = rec;
	return arc;
}

void delarc(arcp __arc) {
	arc_free(__arc);
	delrec(__arc->bk, __arc->p);
}

void arc_free(arcp __arc) {
	recordp *p = __arc->reg;
	recordp *end = p+0xff;
	recordp rec, tmp;
	while(p != end) {
		if (*p != NULL) {
			rec = *p;
			while(rec != NULL) {
				tmp = rec->fd;
				if (rec->sort == _rec_arc)
					arc_free((arcp)rec->p);
				free(rec);
				rec = tmp;
			}
		}
		p++;
	}

	free(__arc->reg);
}

# include "arc.h"
# include <malloc.h>
void tree(arcp __root) {
	recordp *p = __root->reg;
	recordp *end = p+0xff;
	recordp rec;
	while(p != end) {
		if (*p != NULL) {
			rec = *p;
			while(rec != NULL) {
				if (rec->sort == _rec_arc) {
					printf("--> arc: recno{%u}\n", rec->no);
					tree((arcp)rec->p);
				} else {
					printf("--> recno{%u}\n", rec->no);
				}
				rec = rec->fd;
			}
		}
		p++;
	}
}

int main() {
	struct arc root;
	arc_prepare(&root);

	arcp a0 = creatarc(&root, 0);
	arcp a1 = creatarc(a0, 0);
	arcp a2 = creatarc(a1, 0);
	arcp a3 = creatarc(a2, 0);
	arcp a4 = creatarc(a3, 0);


	mdl_u8_t i = 0;
	while(i != 24) {
		creatrec(a4, i++, NULL, _rec_raw);
	}

	tree(&root);

	i = 0;
	while(i != 24) {
		delrec(a4, arc_lookup(a4, i++));
	}
	delarc(a4);
	delarc(a3);
	delarc(a2);
	delarc(a1);
	delarc(a0);

	arc_free(&root);
}

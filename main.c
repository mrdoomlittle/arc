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

# include <string.h>
int main() {
	struct arc root;
	arc_prepare(&root);

	arcp a0 = creatarc(&root, 0);
	arcp a1 = creatarc(a0, 0);
	arcp a2 = creatarc(a1, 0);
	arcp a3 = creatarc(a2, 0);
	arcp a4 = creatarc(a3, 0);


	recordp r0 = creatrec(a4, hash_put("doc", 0), rec_alloc(20), _rec_raw); 
	recordp r1 = creatrec(a4, hash_put("src", 1), rec_alloc(20), _rec_raw);

	char buf[20];
	strcpy(buf, "21299");
	writerec(r0, buf, 0, 20);

	tree(&root);

	bzero(buf, 20);
	readrec(r0, buf, 0, 20);
	printf("%s\n", buf);

	delrec(a4, arc_lookup(a4, hash_get("doc")));
	delrec(a4, arc_lookup(a4, hash_get("src")));

	delarc(a4);
	delarc(a3);
	delarc(a2);
	delarc(a1);
	delarc(a0);

	arc_free(&root);
}

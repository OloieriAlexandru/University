#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/wait.h>
#define NMAX 100005

struct lNode
{
	int v;
	struct lNode *urm;	
};
struct lNode* g[NMAX];
int use[NMAX], size[NMAX];

int root;
int x, y;

void add(struct lNode** node, int value)
{
	struct lNode *newNode = malloc(sizeof(struct lNode));
	newNode -> v = value;
	newNode -> urm = NULL;
	if (!(*node))
	{
		*node = newNode;
		return;
	}
	struct lNode *p = *node;
	struct lNode *last = NULL;
	while (p)
	{
		last = p;
		p = p->urm;
	}
	last->urm = newNode;
}

void printList(struct lNode *node)
{
	while (node)
	{
		printf("%d ", node->v);
		node = node->urm;
	}
	printf("\n");
}

void printArrows(int k)
{
	int i;
	for (i=1;i<=k;++i)
		printf("->");
}

void printLevel(int x, int k)
{
	printArrows(k);
	printf("%d\n", x);
}

void dfs(int k, int nivel)
{
	int i, act;
	struct lNode *p = g[k];
	use[k] = 1;
	printLevel(k,nivel);
	for (i=0;i<size[k];++i)
	{
		act = p->v;
		if (!use[act])	dfs(act, nivel+1);
		p = p->urm;
	}
}

int main(int argc, char *argv[])
{	
	if (argc == 1)
	{
		fprintf(stderr, "Programul trebuie apelat cu un argument: numele fisierului unde se gaseste descrierea arborelui\n");
		return 1;
	}

	FILE *fin = fopen(argv[1], "r");

	if (!fin)
	{
		fprintf(stderr, "Argumentul nu este un fisier valid\n");
		return 2;
	}

	fscanf(fin,"%d",&root);
	fscanf(fin,"%d-%d",&x,&y);

	while (fscanf(fin,"%d-%d",&x,&y)!=EOF)
	{
		add(&g[x],y);
		add(&g[y],x);
		++size[x], ++size[y];
	}

	fclose(fin);

	dfs(root, 1);

	return 0;
}

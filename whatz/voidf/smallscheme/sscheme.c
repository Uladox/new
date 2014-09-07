#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>


typedef enum {list, atom, funct, end} cell_t;
struct cell
{
	cell_t type_cell;
	union content{
		struct cell *cellist;	 
		wchar_t *str;
		struct cell *(*ptrfunct)(struct cell *);
	} stuff;
};

size_t list_size(struct cell *arglist)
{
	struct cell *tmptr = arglist->stuff.cellist;
	if(tmptr == NULL)
		return 0;
	size_t result;
	for(result = 1; tmptr->type_cell != end; tmptr++)
		result++;
	return result;
}

void free_all(struct cell *freeable, int free_here, int first)
{
	if(freeable->type_cell == atom){
		if(freeable->stuff.str != NULL){
			free(freeable->stuff.str);
			freeable->stuff.str = NULL;
		}
	}

	if(freeable->type_cell == list){
		struct cell *rstpnt = freeable->stuff.cellist;
		while(freeable->stuff.cellist->type_cell != end)
			freeable->stuff.cellist++;
		while(freeable->stuff.cellist != rstpnt){
			free_all(freeable->stuff.cellist,0, 1);
			freeable->stuff.cellist--;
		}
		if(first == 1)
			free_all(freeable->stuff.cellist,1,1);
	}

	if(free_here == 1)
		free(freeable);
}

struct cell *eval(struct cell *argcell)
{
	struct cell *result = argcell->stuff.cellist->stuff.ptrfunct(argcell->stuff.cellist + 1);
//	free(argcell);
	return result;
}

struct cell *quote(struct cell *argcell)
{
	return argcell;
}

struct cell *car(struct cell *argcell)
{
	struct cell *result = malloc(sizeof(struct cell));
	*result = *(eval(argcell)->stuff.cellist);
	
	struct cell *freer = argcell->stuff.cellist + 1;
	while(freer->type_cell != end){
		free_all(freer, 0, 0);
		freer++;
	}
	free_all(freer, 0, 0);
	free(argcell->stuff.cellist);
	return result;
}

struct cell *new_funct(struct cell *(*funct_name)(struct cell *))
{
	struct cell *result = malloc(sizeof(struct cell));
	result->type_cell = funct;
	result->stuff.ptrfunct = funct_name;
	return result;
}

struct cell *new_atom(wchar_t *argstr)
{
	struct cell *result = malloc(sizeof(struct cell));
	result->type_cell = atom;
	result->stuff.str = malloc(sizeof(wchar_t) * (wcslen(argstr) + 1));
	wcscpy(result->stuff.str, argstr);
	return result;
}

struct cell *new_list(void)
{
	struct cell *result = malloc(sizeof(struct cell));
	result->type_cell = list;
	result->stuff.cellist = malloc(sizeof(struct cell));
	result->stuff.cellist->type_cell = end;
	return result;
}

void insert_atom(struct cell *arglist, struct cell *argatom)
{
	//too lazy to make new pointer to keep names short
	//but I'll later claim that this is more efficient
	arglist->stuff.cellist = realloc(arglist->stuff.cellist, (sizeof(struct cell)) * (list_size(arglist) + 1));
	struct cell *tmptr = arglist->stuff.cellist;
	while(tmptr->type_cell != end)
		tmptr++;
	*tmptr = *argatom;
	tmptr++;
	tmptr->type_cell = end;
}

void insert_atom_rel(struct cell *arglist, struct cell *argatom)
{
	insert_atom(arglist, argatom);
	free(argatom);
}

int main(int argc, char *argv[])
{
/*
        struct cell *hmm = malloc (sizeof (struct cell));
	hmm->stuff.cellist = NULL;
	hmm->stuff.str = malloc(sizeof(wchar_t) * 3);
	hmm->type_cell = atom;
	wcscpy(hmm->stuff.str, L"no");
*/
/*
	struct cell *amm = malloc (sizeof (struct cell));
	amm->stuff.cellist = malloc(sizeof(struct cell)*2);

	amm->stuff.cellist->stuff.cellist = NULL;
	amm->stuff.cellist->stuff.str = malloc(sizeof(wchar_t) * 3);
	wcscpy(amm->stuff.cellist->stuff.str, L"no");
	++(amm->stuff.cellist);
	amm->stuff.cellist->type_cell = end;
	--(amm->stuff.cellist);
	amm->type_cell = list;
	amm->stuff.cellist->type_cell = atom;
	free_all(amm, 1);
*/
		wprintf(L"Hi!");
	struct cell *a = new_list();
	struct cell *b = new_funct(&car);
	struct cell *c = new_list();
	struct cell *d = new_funct(&quote);
	struct cell *e = new_list();
	struct cell *f = new_atom(L"dsdsds");
	
	insert_atom_rel(e, f);
	insert_atom_rel(c, d);
	insert_atom_rel(c, e);
	insert_atom_rel(a, b);
	insert_atom_rel(a, c);
	
	struct cell *g = eval(a);
	wprintf(L"%ls", g->stuff.str);
	free_all(g, 1, 1);
	return 0;
}
//Notes: Perl glue, Guile, Agda, Rails

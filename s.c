#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>


typedef enum {list, atom, funct, end} cell_t;
struct cell
{
	cell_t type_cell;
	struct cell *next;
	struct cell *prev;
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
	for(result = 1; tmptr->type_cell != end; tmptr = tmptr->next)
		result++;
	return result;
}

void free_all(struct cell *freeable)
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
			freeable->stuff.cellist = freeable->stuff.cellist->next;
		while(freeable->stuff.cellist != rstpnt){
			free_all(freeable->stuff.cellist);
			freeable->stuff.cellist = freeable->stuff.cellist->prev;
		}
			free_all(freeable->stuff.cellist);
	}

	free(freeable);
	freeable = NULL;
}

struct cell *new_atom(wchar_t *argstr)
{
	struct cell *result = malloc(sizeof(struct cell));
	result->type_cell = atom;
	result->stuff.str = malloc(sizeof(wchar_t) * (wcslen(argstr) + 1));
	wcscpy(result->stuff.str, argstr);
	return result;
}

struct cell *new_funct(struct cell *(*funct_name)(struct cell *))
{
	struct cell *result = malloc(sizeof(struct cell));
	result->type_cell = funct;
	result->stuff.ptrfunct = funct_name;
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
	struct cell *tmptr = arglist->stuff.cellist;
	while(tmptr->type_cell != end)
		tmptr = tmptr->next;
	*tmptr = *argatom;
	tmptr->next = malloc(sizeof(struct cell));
	tmptr->next->prev = tmptr;
	tmptr->next->type_cell = end;
}

void insert_atom_rel(struct cell *arglist, struct cell *argatom)
{
	insert_atom(arglist, argatom);
	free(argatom);
	argatom = NULL;
}

struct cell *eval(struct cell *argcell)
{
	struct cell *result = argcell->stuff.cellist->stuff.ptrfunct(argcell->stuff.cellist);
	free(argcell);
	argcell = NULL;
	return result;
	       
}

struct cell *quote(struct cell *argcell)
{
	struct cell *result = argcell->next;

	struct cell *trash = result->next;
	while(trash->type_cell != end){
		struct cell *temp = trash;
		trash = trash->next;
		free_all(temp);
		temp = NULL;
	}
	//frees end of list
	free_all(trash);

	free(argcell);
	argcell = NULL;
//	getchar();
//	wprintf(L"%ls", argcell->next->stuff.cellist->stuff.str);
//	getchar();
//		getchar();
	return result;
}

struct cell *car(struct cell *argcell)
{
	//frees end point (must be done before eval to stop
	//invalid read
	free(argcell->next->next);
	struct cell *to_list = eval(argcell->next);
	//this frees the function cell
	free(argcell);
	struct cell *result = to_list->stuff.cellist;
	//List can be freed because I only need first element
	free(to_list);
	to_list = NULL;
	struct cell *trash = result->next;
	while(trash->type_cell != end){
		struct cell *temp = trash;
		trash = trash->next;
		free_all(temp);
		temp = NULL;
	}
	//frees end of list
	free(trash);
	return result;
}

int main(int argc, char *argv[])
{
	struct cell *a = new_atom(L"sdsdd"); //fo
	struct cell *b = new_list(); //fl
	struct cell *c = new_list(); //sl
	struct cell *d = new_funct(&quote); //quote
	struct cell *e = new_list(); //tl
	struct cell *f = new_funct(&car); //car
	struct cell *g = new_list(); //fl
	struct cell *h = new_funct(&car); //car
	struct cell *i = new_list(); //fl	

	insert_atom_rel(b,a);
	insert_atom_rel(c,b);

	insert_atom_rel(e,d);
	insert_atom_rel(e,c);

	insert_atom_rel(g,f);
	insert_atom_rel(g,e);

	insert_atom_rel(i,h);
	insert_atom_rel(i,g);
	
	struct cell *t = eval(i);
	wprintf(L"%ls", t->stuff.str);
	free_all(t);
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

struct Nobj{
	
	size_t size;
	struct Nobj *Ntype;
	struct Nobj **down;
	struct Nobj *lastdown;
	wchar_t *sym_val;
	struct Nobj *(*data)(struct Nobj *);

}Ntype_vt,Symbol_vt,Mutable_vt,Funct_vt,Empty_vt,
car;

void set_up_types(struct Nobj argobj, wchar_t[] argstr)
{
	argobj.Ntype = &Ntype_vt;
	argobj.down = NULL;
	argobj.lastdown = NULL;
	argobj.size = 0;
	argobj.sym_val = malloc(sizeof(wchar_t)*(wcslen(argstr)+1);
	wcscpy(argobj.sym_val, argstr);
	argobj.data = NULL;
}

int Nobj_length(struct Nobj *arglist)
{
	if(arglist->down == NULL)
		return 0;

	struct Nobj **counter = arglist->down;
	int i;
	for(i = 0; *counter != &Empty_vt; ++counter)
		++i;
	return i;
}

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

struct Nobj{
	
	size_t size;
	struct Nobj *Ntype;
	struct Nobj **down;
	struct Nobj **lastdown;
	wchar_t *sym_val;
	struct Nobj *(*data)(struct Nobj *);

}Ntype_vt,Symbol_vt,Mutable_vt,Funct_vt,Empty_vt,
car;
//first row = types
//second row = base functions

struct Nobj *make_nobj(wchar_t *sym_val)
{
	struct Nobj *nobjptr = malloc(sizeof(struct Nobj));
	nobjptr->Ntype = &Symbol_vt;
	*nobjptr->down = NULL;
	nobjptr->lastdown = NULL;
	nobjptr->size = 0;
	nobjptr->sym_val = malloc(sizeof(wchar_t) * (wcslen(sym_val)+1));
	wcscpy(nobjptr->sym_val, sym_val);
	nobjptr->data = NULL;
}

struct Nobj *get_car(struct Nobj *argobj)
{
	return *argobj->down;
}
void Nninit_types(void)
{
	Ntype_vt.Ntype = &Ntype_vt;
	Ntype_vt.down = NULL;
	Ntype_vt.lastdown = NULL;
	Ntype_vt.size = 0;
	Ntype_vt.sym_val = malloc(sizeof(wchar_t)*9);
	wcscpy(Ntype_vt.sym_val, L"Ntype_vt");
	Ntype_vt.data = NULL;

	Symbol_vt.Ntype = &Ntype_vt;
	Symbol_vt.down = NULL;
	Symbol_vt.lastdown = NULL;
	Symbol_vt.size = 0;
	Symbol_vt.sym_val = malloc(sizeof(wchar_t)*10);
	wcscpy(Symbol_vt.sym_val, L"Symbol_vt");
	Symbol_vt.data = NULL;

	Mutable_vt.Ntype = &Ntype_vt;
	Mutable_vt.down = NULL;
	Mutable_vt.lastdown = NULL;
	Mutable_vt.size = 0;
	Mutable_vt.sym_val = malloc(sizeof(wchar_t)*11);
	wcscpy(Mutable_vt.sym_val, L"Mutable_vt");
	Mutable_vt.data = NULL;

	Funct_vt.Ntype = &Ntype_vt;
	Funct_vt.down = NULL;
	Funct_vt.lastdown = NULL;
	Funct_vt.size = 0;
	Funct_vt.sym_val = malloc(sizeof(wchar_t)*11);
	wcscpy(Funct_vt.sym_val, L"Funct_vt");
	Funct_vt.data = NULL;

	Empty_vt.Ntype = &Ntype_vt;
	Empty_vt.down = NULL;
	Empty_vt.lastdown = NULL;
	Empty_vt.size = 0;
	Empty_vt.sym_val = malloc(sizeof(wchar_t)*2);
	wcscpy(Empty_vt.sym_val, L"t");
	Empty_vt.data = NULL;

}

void Nninit_functions(void)
{
	car.Ntype = &Funct_vt;
	car.down = NULL;
	car.lastdown = NULL;
	car.sym_val = malloc(sizeof(wchar_t)*4);
	wcscpy(car.sym_val, L"car");
	car.data = &get_car;
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

void push_back_Nobj(struct Nobj *arglist, struct Nobj *argobj)
{
	arglist->size++;
	if(arglist->down == NULL){
		arglist->down = malloc(sizeof(struct Nobj *) * 2);
		//important, prevents NULL deref
		*(arglist->down) = argobj;
		**(arglist->down) = *argobj;
		*(++arglist->down) = &Empty_vt;
		arglist->lastdown = (--arglist->down);
	}
	else{
		arglist->down = realloc(arglist->down, sizeof(struct Nobj *) * (arglist->size + 1));
		*(++arglist->lastdown) = argobj;
		**(arglist->lastdown) = *argobj;
		*(++arglist->lastdown) = &Empty_vt;
		arglist->lastdown--;		
	}
	
}
/*
struct Nobj *lisp_eval(struct Nobj *arglist)
{
//	resolve(arglist->down);
	arglist->down.data(arglist->down[1]);
}
*/
int main(int argc, char *argv[])
{
/*
	Nninit_types();
	Nninit_functions();
	struct Nobj *o = malloc(sizeof(struct Nobj));
	o->size = 0;
	o->down = NULL;
	o->lastdown = NULL;

	push_back_Nobj(o, &Ntype_vt);
	push_back_Nobj(o, &Funct_vt);
	push_back_Nobj(o, &Mutable_vt);
	wprintf(L"%ls\n", (*(o->down + 3))->sym_val);

	wprintf(L"%i\n", Nobj_length(o));
*/

	wchar_t * ayy = malloc(sizeof(wchar_t) * 4);
	wcscpy(ayy, L"abc");
	wprintf(L"%ls", ayy + 1);
}


/*
	int b = 5;
	void *a;
	a = &hello;
	((void (*)())a)();
	a = &b;
	printf("%i", *((int*) a));
	return 0;

void hello(void){
	printf("hello world");
}
*/

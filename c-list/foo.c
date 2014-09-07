#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
//gcc -rdynamic -o foo foo.c -ldl
int main(int argc, char **argv)
{
    void *handle;
    char *error;
    char *(*fptr)(void);

   handle = dlopen("sl/libtest.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

   dlerror();    
*(void **)(&fptr) = dlsym(handle, "test");

  if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

printf("%s\n", (*fptr)());
    dlclose(handle);
    exit(EXIT_SUCCESS);
}

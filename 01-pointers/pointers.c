#include<stdio.h>

int main (void){
    int x = 3;
    int *p = &x;
    int **pp = &p;
    printf ("x     = %d\n", x);
    printf ("x(address) = %p\n" , &x);
    printf ("p     = %p\n", p);
    printf ("p(address) = %p\n" , &p);
    printf ("pp    = %p\n", pp);
    printf ("pp(address)    = %p\n", &pp);
    printf("====================\n");
    printf("&x    = %p\n", (void*)&x);
    printf("&p    = %p\n", (void*)&p);
    printf("*p    = %d\n", *p);
    printf("*pp   = %p\n", (void*)*pp);
    printf("**pp  = %d\n", **pp);




    return 0;
}
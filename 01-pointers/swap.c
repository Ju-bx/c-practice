#include <stdio.h>

void swap(int *a, int *b){
    int tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(void){
    int x = 3;
    int y = 7;
    printf("%d\n", x);
    printf("%p\n", &x);
    printf("%d\n", y);
    printf("%p\n", &y);
    swap(&x, &y);
    printf("%d\n", x);
    printf("%p\n", &x);
    printf("%d\n", y);
    printf("%p\n", &y);
    return 0;
}
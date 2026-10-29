#include<stdio.h>

int main (){
    int n = 10;
    int *p = &n;
    char c = 'A';
    char *q = &c;
    double d = 1.5;
    double *r = &d;
    printf("n: %d\n", n);
    printf("p: %p\n", (void*)p);
    printf("p+1: %p\n", (void*)(p+1));
    printf("c: %d\n", c);
    printf("c: %c\n", c);
    printf("q: %p\n", (void*)q);
    printf("q+1: %p\n", (void*)(q+1));
    printf("d: %f\n", d);
    printf("r: %p\n", (void*)r);
    printf("r+1: %p\n", (void*)(r+1));

    printf("======================\n");

    int arr[5] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i<sizeof(arr) / sizeof(arr[0]); i++){
        printf("i: %zu\n", i);
        printf("arr[i]: %d\n", arr[i]);
        printf("*(arr+i): %d\n", *(arr+i));
        printf("&arr[i]: %p\n", (void*)&arr[i]);
    }
    printf("arr     = %p\n", (void*)arr);
    printf("&arr[0] = %p\n", (void*)&arr[0]);
    printf("========================\n");
    void print_size(int a[]) {
        printf("in function: %zu\n", sizeof(a));
    }
    int *ptr = arr;
    printf("sizeof(arr) = %zu\n", sizeof(arr));
    printf("sizeof(ptr) = %zu\n", sizeof(ptr));
    print_size(arr);
    printf("================\n");
    #define H 3
    #define W 4
    int img[H * W];
    for (size_t i = 0; i < H;i++){
        for (size_t j = 0;j< W;j++){
            img[i*W+j] = i*10 + j;
        }
    }
    printf("img : \n");
    for (size_t i = 0; i < H;i++){
        for (size_t j = 0;j< W;j++){
            printf("%d",img[i*W+j]);
            if (j+1<4){
                printf(" ");
            }
        }
        printf("\n");
    }
    
}
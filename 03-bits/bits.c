#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t x) {
    int tmp = x;
    for (int i = 7;i >= 0;i--){
        if (1<<i <= tmp){
            printf("%d", 1);
            tmp = tmp - (1<<i);
        }else{
            printf("%d", 0);
        }
    }
    printf("\n");
}
uint8_t set_bit(uint8_t x, int n){
    return (x | (1 << n));
}
uint8_t clear_bit(uint8_t x, int n){
    return (~(~x |(1 << n)));
}
uint8_t toggle_bit(uint8_t x, int n){
    return (x^(1<<n));
}  // 第 n 個 bit 反轉，其他不變
int     check_bit(uint8_t x, int n){
    return (x & (1 << n))!=0;
}

void set_reg(uint8_t reg){
    reg = set_bit(reg, 0);
    reg = set_bit(reg, 3);
    print_binary(reg);
    reg = set_bit(reg, 7);
    print_binary(reg);
    reg = clear_bit(reg, 3);
    print_binary(reg);
    reg = toggle_bit(reg, 0);
    print_binary(reg);
    printf("%d\n", check_bit(reg, 7)); 

}


int main(){
    uint8_t x = 5;
    print_binary(x);                  // 00000101
    print_binary(set_bit(x, 3));      // 00001101
    print_binary(clear_bit(x, 0));    // 00000100
    print_binary(toggle_bit(x, 2));   // 00000001
    print_binary(toggle_bit(x, 1));   // 00000111
    printf("%d\n", check_bit(x, 2));  // 1
    printf("%d\n", check_bit(x, 1));  // 0

    uint8_t reg = 0;   // 一開始全部都暗：00000000
    set_reg(reg);


    return 0;
}
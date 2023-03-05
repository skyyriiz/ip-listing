#include <stdio.h>
#include <math.h>

int main (){
    char large_text[100];
    int i = 0;
    char c;

    printf("\nInput: ");
    for (i = 0;(c = getchar()); i++){
        if ((c == '\0') | (c == 10)) {
            large_text[i] = '\0';
            break;
        }
        large_text[i] = c;
    }
    printf("The input was: ");
    int i2 = 0;
    while (i2 <= i){
        printf("%c", large_text[i2]);
        i2++;
    }
    printf("\n");

    // convert to decimal
    i2 = 0;
    int ten_to_the;
    int value;
    int decimal = 0;
    while (i2 < i){
        ten_to_the = i - 1 - i2;
        value = (large_text[i2] - '0') * (int)(pow(10,ten_to_the));
        printf("char: %c\tdec: %d\t*ten_to_the: %d\tproduct: %d\t",
            large_text[i2], large_text[i2] - '0', ten_to_the, value);
        i2++;
        decimal = decimal + value;
        printf("decimal is: %d\n", decimal);
    }

    // convert to hexadecimal
    char hex[24];
    int slice = decimal;
    int remain;
    int i3 = 0;
    while (slice > 0){
        remain = slice % 16;
        slice = slice / 16;
        switch (remain){
            case 10 : c = 'a'; break;
            case 11 : c = 'b'; break;
            case 12 : c = 'c'; break;
            case 13 : c = 'd'; break;
            case 14 : c = 'e'; break;
            case 15 : c = 'f'; break;
            default : c = remain + '0';
        }
        hex[i3] = c;
        i3++;
    }
    hex[i3] = '\0';
    // reverse the hex string for proper order
    char newhex[24];
    newhex[i3] = '\0';
    for (i = 0; (c = hex[i]); i++){
        newhex[(i3-1)-i] = c;
    }

    // convert to binary
    char bin[100];
    slice = decimal;
    remain = 0;
    i3 = 0;

    while (slice > 0){
        remain = slice % 2;
        slice = slice / 2;
        switch(remain){
            case 0 : c = '0'; break;
            case 1 : c = '1'; break;
            default : c = '!';
        }
        bin[i3] = c;
        i3++;
    }
    bin[i3] = '\0';
    // reverse the string for proper order
    char newbin[100];
    newbin[i3] = '\0';
    for (i = 0; (c = bin[i]); i++){
        newbin[(i3-1)-i] = c;
    }
    printf("==============================================\n");
    printf("Decimal:%d\tHex: 0x%s\tBinary: %s\n", decimal, newhex, newbin);
    printf("==============================================\n");
    printf("Program finished...\n");
    return 0;
}
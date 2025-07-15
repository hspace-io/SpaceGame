#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char data[] = {0, };

typedef struct _ptr1{
    uint16_t *parr;
    uint8_t arr[8];
}PTR1;

typedef struct _ptr2{
    uint8_t arr[8];
    uint16_t *parr;
}PTR2;

int main()
{
    char arr[40] = {0, };
    int * prt;

    //bof
    printf("input: ");
    scanf("%s", arr);

    PTR1 *p1;
    p1 = (PTR1 *)malloc(sizeof(PTR1)*2);

    memset(p1, 0, sizeof(p1));

    printf("input: ");
    scanf("%hd", p1->parr);
    printf("input: ");
    scanf("%s", p1->arr);

    free(p1);

    PTR2 * p2;
    p2 = (PTR2 *)malloc(sizeof(PTR2)*2);
    printf("input");
    scanf("%s", p2->arr);
    printf("input: ");
    scanf("%hd", p2->parr);

    free(p2);

    PTR1 *p3;
    p3 = (PTR1 *)malloc(sizeof(PTR1)*2);

    printf("input: ");
    scanf("%hd", p3->parr);
    printf("input: ");
    scanf("%s", p1->arr);

    free(p1);
    
    
    return 0;
}
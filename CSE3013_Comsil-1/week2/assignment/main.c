#pragma warning(disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "solve.h"

int main()
{
    int N;
    scanf("%d", &N);
    //printf("N: %d\n", N);
    int *arr = calloc(10, sizeof(int));
    solve(1, N, 1, arr);
    
   for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
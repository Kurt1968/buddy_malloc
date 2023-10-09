#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "myalloc.h"

int main() {
    // Test 1: Allocazione di una piccola quantità di memoria con myalloc
    int *ptr1 = myalloc(sizeof(int));
    *ptr1 = 42;
    printf("Test 1: %d\n", *ptr1);

    // Test 2: Allocazione di una grande quantità di memoria con myalloc
    char *ptr2 = myalloc(1024 * sizeof(char));
    strcpy(ptr2, "May the force be with you!");
    printf("Test 2: %s\n", ptr2);

    // Test 3: Tentativo di liberare un puntatore nullo con myfree
    int result3 = myfree(NULL);
    printf("Test 3: myfree(NULL) result = %d\n", result3);

    // Test 4: Tentativo di liberare un puntatore allocato con myalloc
    int result4 = myfree(ptr1);
    printf("Test 4: myfree(ptr1) result = %d\n", result4);

    // Test 5: Tentativo di liberare un puntatore allocato con mmap
    int result5 = myfree(ptr2);
    printf("Test 5: myfree(ptr2) result = %d\n", result5);

    // Test 6: Tentativo di liberare un puntatore già liberato con myfree
    int result6 = myfree(ptr2);
    printf("Test 6: myfree(ptr2) (seconda volta) result = %d\n", result6);

    // Test 7: Allocazione di memoria con myalloc dopo la liberazione
    int *ptr7 = myalloc(sizeof(int));
    *ptr7 = 123;
    printf("Test 7: %d\n", *ptr7);

    // Test 8: Allocazione di una quantità molto grande di memoria con myalloc
    char *ptr8 = myalloc(1024 * 1024 * sizeof(char));
    strcpy(ptr8, "This is a large allocation.");
    printf("Test 8: %s\n", ptr8);

    // Test 9: Tentativo di allocare più memoria di quella disponibile
    char *ptr9 = myalloc(1024 * 1024 * 1024);
    if (ptr9 == NULL) {
        printf("Test 9: Failed to allocate 1 GB\n");
    } else {
        printf("Test 9: Allocated 1 GB\n");
        myfree(ptr9); // Libera la memoria allocata
    }

    // Test 10: Tentativo di liberare un puntatore non allocato con myfree
    int x = 100;
    int *ptr10 = &x;
    int result10 = myfree(ptr10);
    printf("Test 10: myfree(ptr10) result = %d\n", result10);

    //Test 11:saturazione buddy
    printf("Test 11: saturazione buddy\n");
    for(int i=0;i<512;i++){
        int* ptr11=myalloc(100);
    }
    return 0;
}

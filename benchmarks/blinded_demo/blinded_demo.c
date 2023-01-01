#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
// #include "tweetnacl.h"
// #include "randombytes.h"
#include "blinded.h"


#define ARRAY_SIZE (256*8)

volatile unsigned char g_unblinded[10];
volatile unsigned char g_blinded[ARRAY_SIZE];
volatile unsigned char g_blinded2[ARRAY_SIZE];

int main() {
    
    unsigned char x = 0;

    memset(g_unblinded, 0x5a, 10);

    memset(g_blinded, 0x3f, ARRAY_SIZE);
    // printf("Hi hi hi!\n");
    blind(g_blinded, ARRAY_SIZE);
    // printf("Mid\n");
    // if (g_blinded[128] > 1) {
    //     printf("Branched based on blinded value.\n");
    // }
    printf("After\n");

    memset(g_blinded2, 0x2c, ARRAY_SIZE);
    blind(g_blinded2, ARRAY_SIZE);

    int op = 4;
    printf("Test case: %d\n", op);
    switch (op) {
        case 1: // try to read from blinded memory -------------------------------- PASS
            x = g_blinded[0];
            // printf("x = %d\n", x);
            break;

		case 2: // writing into blinded memory ------------------------------------ PASS
            g_blinded[0] = 5;
            g_blinded[1] = g_blinded2[0] + 1;
            // printf("%d %d\n", g_blinded[0], g_blinded[1]);
            break;

		// ########################

        case 3: // try to print from blinded memory ------------------------------- should FAULT
            printf("Printing blinded value: %lx\n", g_blinded[0]);
            printf("INCORRECTLY...");
            break;

        case 4: // branch based on blinded memory --------------------------------- FAULT
            if (g_blinded[0] > 1) {
                printf("Branched based on blinded value.\n");
            }
            printf("INCORRECTLY...");
            break;

        case 5: // try to use blinded memory as address for a load ---------------- FAULT
            x = g_unblinded[g_blinded[1]];
            printf("Used blinded value as addr for load.\n");
            printf("INCORRECTLY...");
            break;

		case 6: // try to use blinded memory as address for a store --------------- FAULT
            g_unblinded[g_blinded[1]] = 4;
            printf("Used blinded value as addr for store.\n");
            printf("INCORRECTLY...");
            break;

        default:
            printf("Incorrect test case number: %d.\n", op);
            return op;
            break;
    }

    printf("PASSED\n");

    // asm volatile (
    //     "rblnd"
    //     ::
    // );
    return 0;
}

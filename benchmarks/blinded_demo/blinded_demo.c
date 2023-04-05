#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
// #include "tweetnacl.h"
// #include "randombytes.h"
#include "blinded.h"


// #define ARRAY_SIZE (256*8)
#define ARRAY_SIZE (40)

volatile unsigned char g_unblinded[10];
volatile unsigned char g_blinded[ARRAY_SIZE];
volatile unsigned char g_blinded2[ARRAY_SIZE];

int main() {
    
    unsigned char x = 0;

    memset(g_unblinded, 0x5a, 10);

    // 16 bytes reserved at start of buffer for nonce;
    // nonce only stored in bottom 96 bits
    // nonce = 0x00 00 00 09 00 00 00 4a 00 00 00 00
    g_blinded[0] = 0x00;
    g_blinded[1] = 0x00;
    g_blinded[2] = 0x00;
    g_blinded[3] = 0x00;
    g_blinded[4] = 0x4a;
    g_blinded[5] = 0x00;
    g_blinded[6] = 0x00;
    g_blinded[7] = 0x00;
    g_blinded[8] = 0x09;
    g_blinded[9] = 0x00;
    g_blinded[10] = 0x00;
    g_blinded[11] = 0x00;
    g_blinded[12] = 0x00;
    g_blinded[13] = 0x00;
    g_blinded[14] = 0x00;
    g_blinded[15] = 0x00;
    memset(g_blinded+16, 0x3f, ARRAY_SIZE-16);

    // printf("Unblinded array (before):");
    // for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
    //     if (i % 4 == 0)
    //         printf("\n");
    //     printf("%x ", g_blinded[i]);
    // }

    // printf("Hi hi hi!\n");
    blind(g_blinded, ARRAY_SIZE);
    // printf("Mid\n");
    // if (g_blinded[128] > 1) {
    //     printf("Branched based on blinded value.\n");
    // }
    printf("After\n");

    g_blinded2[0] = 0x00;
    g_blinded2[1] = 0x00;
    g_blinded2[2] = 0x00;
    g_blinded2[3] = 0x00;
    g_blinded2[4] = 0x4a;
    g_blinded2[5] = 0x00;
    g_blinded2[6] = 0x00;
    g_blinded2[7] = 0x00;
    g_blinded2[8] = 0x09;
    g_blinded2[9] = 0x00;
    g_blinded2[10] = 0x00;
    g_blinded2[11] = 0x00;
    g_blinded2[12] = 0x00;
    g_blinded2[13] = 0x00;
    g_blinded2[14] = 0x00;
    g_blinded2[15] = 0x00;
    memset(g_blinded2+16, 0x2c, ARRAY_SIZE-16);
    blind(g_blinded2, ARRAY_SIZE);

    int op = 5;
    printf("Test case: %d\n", op);
    switch (op) {
        case 1: // try to read from blinded memory -------------------------------- PASS
            x = g_blinded[16];
            // printf("x = %d\n", x);
            break;

		case 2: // writing into blinded memory ------------------------------------ PASS
            g_blinded[16] = 5;
            g_blinded[17] = g_blinded2[16] + 1;
            printf("Overwritten: %d\n", g_blinded[16]);
            break;

		// ########################

        case 3: // try to print from blinded memory ------------------------------- should FAULT
            printf("Printing blinded value: %lx\n", g_blinded[16]);
            printf("INCORRECTLY...");
            break;

        case 4: // branch based on blinded memory --------------------------------- FAULT
            if (g_blinded[16] > 1) {
                printf("Branched based on blinded value.\n");
            }
            printf("INCORRECTLY...");
            break;

        case 5: // try to use blinded memory as address for a load ---------------- FAULT
            x = g_unblinded[g_blinded[17]];
            printf("Used blinded value as addr for load.\n");
            printf("INCORRECTLY...");
            break;

		case 6: // try to use blinded memory as address for a store --------------- FAULT
            g_unblinded[g_blinded[17]] = 4;
            printf("Used blinded value as addr for store.\n");
            printf("INCORRECTLY...");
            break;

        default:
            printf("Incorrect test case number: %d.\n", op);
            return op;
            break;
    }

    printf("PASSED\n");

    printf("Unblinding...\n");

    unblind(g_blinded, ARRAY_SIZE);

    printf("Unblinded array (after):");
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        if (i % 4 == 0)
            printf("\n");
        printf("%x ", g_blinded[i]);
    }

    printf("Done\n");

    // asm volatile (
    //     "rblnd"
    //     ::
    // );
    return 0;
}

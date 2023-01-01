#ifndef BLINDED_H
#define BLINDED_H

#include "rocc.h"

// size must be multiple of 8
void blind(char *x, unsigned long long int size) {
	ROCC_INSTRUCTION_SS(0, x, size, 0);
    asm volatile ("fence\n\t");
}

// size must be multiple of 8
void unblind(char *x, unsigned long long int size) {
	ROCC_INSTRUCTION_SS(0, x, size, 0);
    asm volatile ("fence\n\t");
}

#endif
#ifndef INITIALISATION_H
#define INITIALISATION_H

#include <stdint.h>
#include <stdio.h>
#include "vm.h"
int load_memory(const char *filename, uint8_t *instruction_mem, uint8_t *data_mem);
int initialise_heap_banks(VM *vm);
#endif
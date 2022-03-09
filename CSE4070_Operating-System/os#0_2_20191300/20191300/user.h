#ifndef __LIB_KERNEL_USER_H
#define __LIB_KERNEL_USER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"

// struct of list
struct list_item {
	struct list_elem elem;
	int data;
};

// struct of hash
struct hash_item {
	struct hash_elem elem;
	int data;
};

struct bitmap* myBitmap[10];
struct hash* myHash[10];
struct list* myList[10];


void tok(char* str, char** arg);
void level_1(char** arg);
void level_2(char** arg);

#endif
#ifndef _LOGICEXPRESSION_H_INCLUDE_
#define _LOGICEXPRESSION_H_INCLUDE_

#include <stdint.h>

#define MAXSIZE 100

struct Stack
{
	char ops[MAXSIZE];
	int order[MAXSIZE];
	int currentIndex;
};

void initialize(struct Stack *stack);
void push(struct Stack *stack, char op, int order);
char pop(struct Stack *stack);
struct Stack inToPostfix(char *expr);
void truthTable(struct Stack operands, uint8_t* table);

#endif

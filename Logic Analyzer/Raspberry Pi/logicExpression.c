#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logicExpression.h"

#define MAXSIZE 100

void initialize(struct Stack *stack)
{
	stack->currentIndex = -1;
}

void push(struct Stack *stack, char operate, int order)
{
	stack->currentIndex++;
	stack->ops[stack->currentIndex] = operate;
	stack->order[stack->currentIndex] = order;
}

char pop(struct Stack *stack)
{
	char popChar;
	popChar = stack->ops[stack->currentIndex];
	stack->currentIndex--;
	return popChar;
}

struct Stack inToPostfix(char * expr)
{
	struct Stack operators;
	struct Stack operands;	
	initialize(&operators);
	initialize(&operands);
	
	int i = 0;
	int j = 0;
	int parser = 0;
	
	char temp = 0;
	
	for (i = 0; expr[i] != '\0'; i++)
	{
		if (expr[i] == 'A' || expr[i] == 'B' || expr[i] == 'C' || expr[i] == 'D' || 
			expr[i] == 'E' || expr[i] == 'F' || expr[i] == 'G' || expr[i] == 'H')
		{
			push(&operands, expr[i], 0);
		}
		else if(expr[i] == '~')
		{
			if (operators.order[operators.currentIndex] == 3 && parser == 0)
			{
				temp = pop(&operators);
				push(&operands, temp, 0);					
			}
			push(&operators, expr[i], 3);
		}
		else if(expr[i] == '&')
		{
			for (j = 0; j < 1; j++)
			{
				if (operators.order[operators.currentIndex] >= 2 && parser == 0)
				{
					temp = pop(&operators);
					push(&operands, temp, 0);
					/*
					if (operators.currentIndex - 1 != 0)
					{
						temp = pop(&operators);
						push(&operands, temp, 0);
					}
					*/
				}
			}
			push(&operators, expr[i], 2);
		}
		else if (expr[i] == '|')
		{
			for (j = 0; j < 2; j++)
			{
				if (operators.order[operators.currentIndex] >= 1 && parser == 0)
				{
					temp = pop(&operators);
					push(&operands, temp, 0);
					/*
					if (operators.order[operators.currentIndex] != 0)
					{
						temp = pop(&operators);
						push(&operands, temp, 0);
					}
					*/
				}
			}
			push(&operators, temp, 1);
		}
		else if (expr[i] == '(')
		{
			push(&operators, expr[i], 4);
			parser++;
		}
		else if (expr[i] == ')')
		{
			while (operators.ops[operators.currentIndex] != '(')
			{
				temp = pop(&operators);
				push(&operands, temp, 0);
			}
			temp = pop(&operators);
			parser--;
		}

	}
	
	temp = pop(&operators);
	push(&operands, temp, 0);
		
	int stackSize = operands.currentIndex;
	for (i = 0; i <= stackSize; i++)
	{
		printf("%c", operands.ops[i]);
	}
	printf("\n");	
	
	
		
	return operands;	
}

void truthTable(struct Stack operands, uint8_t* table)
{
	int stackSize = operands.currentIndex;
	int k = 0;
	int l = -1;
	unsigned int array[50];
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t F;
	uint8_t G;
	uint8_t H;
	
	uint8_t op1;
	uint8_t op2;
	uint8_t result;
	
	char outputTable[50];
	char binOut[3];
	printf("H G F E D C B A| OUTPUT\n");
	
	int i = 0;
	int mask = 1;
	for (i = 0; i < 256; i++)
	{
		A = (i & (1 << 0)) >> 0;
		B = (i & (1 << 1)) >> 1;
		C = (i & (1 << 2)) >> 2;
		D = (i & (1 << 3)) >> 3;
		E = (i & (1 << 4)) >> 4;
		F = (i & (1 << 5)) >> 5;
		G = (i & (1 << 6)) >> 6;
		H = (i & (1 << 7)) >> 7;

		while (k <= stackSize)
		{
			if (operands.ops[k] == 'A')
			{
				l++;
				array[l] = A;
			}
			else if (operands.ops[k] == 'B')
			{
				l++;
				array[l] = B;
			}
			else if (operands.ops[k] == 'C')
			{
				l++;
				array[l] = C;
			}
			else if (operands.ops[k] == 'D')
			{
				l++;
				array[l] = D;
			}
			else if (operands.ops[k] == 'E')
			{
				l++;
				array[l] = E;
			}
			else if (operands.ops[k] == 'F')
			{
				l++;
				array[l] = F;
			}
			else if (operands.ops[k] == 'G')
			{
				l++;
				array[l] = G;
			}
			else if (operands.ops[k] == 'H')
			{
				l++;
				array[l] = H;
			}
			else if (operands.ops[k] == '~')
			{
				if (array[l] == 1)
				{
					array[l] = 0;
				}
				else
				{
					array[l] = 1;
				}
			}
			else if (operands.ops[k] == '&')
			{
				op2 = array[l];
				l--;
				op1 = array[l];
				result = op2 & op1;
				array[l] = result;
			}
			else if (operands.ops[k] == '|')
			{
				op2 = array[l];
				l--;
				op1 = array[l];
				result = op2|op1;
				array[l] = result;
			}
			k++;
		}	
		table[i] = array[l];
		printf("%d %d %d %d %d %d %d %d| %d\n", H, G, F, E, D, C, B, A, table[i]);
		
		k = 0;
		l = -1;
	}
	return;
}



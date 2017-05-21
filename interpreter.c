#include "interpreter.h"
#include <stdio.h>

void execute(struct AstElement* ast)
{
	printf("TODO: Execute...\n");
	if(ast->kind==ekStatements)
	{
		printf("statements\n");
	}
}


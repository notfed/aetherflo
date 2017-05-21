#include "interpreter.h"
#include "critbit.h"
#include <stdio.h>

void interpret_statements(AstElement* ast)
{
		execute(ast->data.statements.thisStatement);
		execute(ast->data.statements.childStatements);
}
void interpret_print(AstElement* ast)
{
		printf("Print\n");
}
void interpret_conditional(AstElement* ast)
{
		printf("Conditional\n");
}
void interpret_assignment(AstElement* ast)
{
		char* name = ast->data.assignment.name->data.name;
		printf("Assignment to %s\n", name);
}

void execute(struct AstElement* ast)
{
	switch(ast->kind) 
	{
		case ekStatements: interpret_statements(ast); break;
		case ekPrint: interpret_print(ast); break;
		case ekAssignment: interpret_assignment(ast); break;
		case ekConditional: interpret_conditional(ast); break;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

#include "parser.h"
#include "eval.h"

int main(void)
{
	init_parser();
	puts("clisp v0.0");

	for (;;)
	{
		char* input = readline("->> ");
		add_history(input);

		if (!strcmp(input, "quit"))
		{
			free(input);
			break;
		}

		mpc_ast_t* ast = parse(input);

		if (!ast)
		{
			free(input);
			continue;
		}

		printf("%d\n", eval(ast));
		mpc_ast_delete(ast);
		free(input);
	}

	clear_history();
	mpc_cleanup(4, Number, Operator, Expr, Lisp);
	return 0;
}



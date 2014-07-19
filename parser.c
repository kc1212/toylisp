
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "common.h"

static int all_isspace(const char* input)
{
	while (isspace(*input))
		input++;

	if (*input== '\0')
		return 1;

	return 0;
}

void init_parser()
{
	Integer		= mpc_new("integer");
	Float		= mpc_new("float");
	Operator	= mpc_new("operator");
	Expr		= mpc_new("expr");
	Lisp		= mpc_new("lisp");

	mpca_lang(MPCA_LANG_DEFAULT,
		"integer	: /-?[0-9]+/ ;"
		"float		: /-?[0-9]*\\.[0-9]+/ | /-?[0-9]+\\.[0-9]*/ ;" // TODO '1.' does not match
		"operator	: '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\" ;"
		"expr		: <integer> | <float> | '(' <operator> <expr>+ ')' ;"
		"lisp		: /^/ <operator> <expr>+ /$/ ;",
		Integer, Float, Operator, Expr, Lisp);
}

// abstract syntax tree
mpc_ast_t* parse(const char* input)
{
	mpc_result_t r;
	if (mpc_parse("<stdin>", input, Lisp, &r))
	{
		log_info_to(logfp, "Parsing successful: %s", input);
		mpc_ast_print_to(r.output, logfp);
		return r.output;
	}
	else if (all_isspace(input))
	{
		log_info_to(logfp, "Parsing failed: %s (empty) ", input);
		mpc_err_delete(r.error);
	}
	else
	{
		log_info_to(logfp, "Parsing failed: %s", input);
		mpc_err_print_to(r.error, stderr);
		mpc_err_delete(r.error);
	}
	return NULL;
}




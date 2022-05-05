/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
atlanta_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern atlanta_int line;
extern Token tokenizer();
extern atlanta_chr* keywordTable[];

#define STR_LANGNAME "atlanta"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */
/*
	"if",
	"elseif",
	"else", "repeat",
	"while",
	"function",
	"for",
	"in",
	"continue",
	"done",
	"true",
	"false",
	"null",
	"inf",
	"NA",
	"return",
	"or",
	"and",
	"double",
	"int",
	"string"*/
/* Constants */
enum KEYWORDS {
	IF,
	ELSEIF,
	ELSE,
	REPEAT,
	WHILE,
	FUNCTION,
	FOR,
	IN,
	CONTINUE,
	DONE,
	TRUE,
	FALSE,
	NUll,
	INF,
	NA,
	RETURN,
	OR,
	AND,
	DOUBLE,
	INT,
	STRING,
	NO_ATTR = -1
};

/* Function definitions */
atlanta_nul startParser();
atlanta_nul matchToken(atlanta_int, atlanta_int);
atlanta_nul syncErrorHandler(atlanta_int);
atlanta_nul printError();

/* TODO: Place ALL non-terminal function declarations */
atlanta_nul assignmentExpression();
atlanta_nul assignmentStatement();
atlanta_nul optionalStatements();
atlanta_nul statement();
atlanta_nul statements();
atlanta_nul statementsPrime();
atlanta_nul arithmatic_expression();
atlanta_nul unary_arithmatic_expression();
atlanta_nul additive_arithmatic_expression();
atlanta_nul additive_arithmatic_expression_prime();
atlanta_nul multiplicative_arithmatic_expression();
atlanta_nul multiplicative_arithmatic_expression_prime();
atlanta_nul string_expression();
atlanta_nul primary_arithmatic_expression();
atlanta_nul function_expression();

atlanta_nul variable_list();
atlanta_nul variable_identifier();
atlanta_nul variable_list_prime();
atlanta_nul output_statement();
atlanta_nul output_list();
atlanta_nul primary_string_expression();
atlanta_nul string_expression_prime();

#endif

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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A22, A32.
* Date: Feb 21 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VMID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 20   /* maximum number of digits for IL */
#define FNUM_LEN 20  /*maximum number of digits for FL*/

#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MVID_T,		/*  1: Method and variable id token*/
	STR_T,		/*  2: String literal token */
	IL_T,       /*  3: Integer literal token*/
	FL_T,       /*  4: Float literal token*/
	COM_T,      /*  3: Comma token*/
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	LBR_T,		/*  5: Left brace token */
	RBR_T,		/*  6: Right brace token */
	LBK_T,		/*  7: Left bracket token */
	RBK_T,		/*  8: Right bracket token */
	KW_T,		/*  9: Keyword token */
	EOS_T,		/* 10: End of statement (semicolon) */
	RTE_T,		/* 11: Run-time error token */
	SEOF_T,		/* 12: Source end-of-file token */
	ASS_T,		/* 13: Assignment token*/
    ART_OP_T,   /* 14: Arithmetic operator token */
    REL_OP_T,   /* 15: Relational operator token */ 
    LOG_OP_T    /* 16: Logical operator token */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW} AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	atlanta_int codeType;				/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	atlanta_int intValue;						/* integer literal attribute (value) */
	atlanta_int keywordIndex;					/* keyword index in the keyword table */
	atlanta_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	atlanta_flt floatValue;					/* floating-point literal attribute (value) */
	atlanta_chr idLexeme[VMID_LEN + 1];		/* variable identifier token attribute */
	atlanta_chr errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	atlanta_flg atlanta;			/* Flags information */
	union {
		atlanta_int intValue;				/* Integer value */
		atlanta_flt floatValue;			/* Float value */
		atlanta_nul* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	atlanta_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## , 
 *  .&., .|. , .!. , SEOF.
 */

/* TO_DO: Error states and illegal state */
#define ES  10		/* Error state with no retract */
#define ER  11		/* Error state with retract */
#define IS -1		/* Illegal state */

#define TABLE_COLUMNS 7

/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '.'
#define CHRCOL4 '\"'

/* These constants will be used on VID / MID function */
/*#define MNIDPREFIX '&'*/

static atlanta_int transitionTable[][TABLE_COLUMNS] = {
/*   [A-z]  [0-9]    _     .     "    other  SEOF
* 
	   L(0), D(1), U(2), P(3), Q(4),  O(5),  E(6) */
	{     6,    1,   ES,   ES,    8,   ES,   ER}, /*S0: NOAS*/
	{     2,    1,    2,    3,    2,    2,    2}, /*S1: NOAS*/
	{    IS,   IS,   IS,   IS,   IS,   IS,   IS}, /*S2: ASWR (IL)*/
	{    ES,    4,   ES,   ES,   ES,   ES,   ER}, /*S3: NOAS*/
	{     5,    4,    5,    5,    5,    5,    5}, /*S4: NOAS*/
	{    IS,   IS,   IS,   IS,   IS,   IS,   IS}, /*S5: ASWR (FL)*/
	{     6,    6,    6,    6,    7,    7,    7}, /*S6: NOAS*/
	{    IS,   IS,   IS,   IS,   IS,   IS,   IS}, /*S7: ASWR (VMID)*/
	{     8,    8,    8,    8,    9,    8,    9}, /*S8: NOAS*/
	{    IS,   IS,   IS,   IS,   IS,   IS,   IS}, /*S9: ASNR (SL)*/
	{    IS,   IS,   IS,   IS,   IS,   IS,   IS}, /*S10: ASNR (ES)*/
	{    IS,   IS,   IS,   IS,   IS,   IS,   IS}  /*S11: ASWR (ER)*/
};

/* Defining accepting state types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static atlanta_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASWR, /* 02 (IL)*/
	NOAS, /* 03 */
	NOAS, /* 04 */
	ASWR, /* 05 (FL)*/
	NOAS, /* 06 */
	ASWR, /* 07 (VMID) - variables and methods */
	NOAS, /* 08 */
	ASNR, /* 09 (SL) */
	ASNR, /* 10 (Err1 - no retract) */
	ASWR, /* 11 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
atlanta_int startScanner(BufferPointer psc_buf);
static atlanta_int nextClass(atlanta_chr c);			/* character class function */
static atlanta_int nextState(atlanta_int, atlanta_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(atlanta_chr* lexeme);

/* Declare accepting states functions */
Token funcIL    (atlanta_chr lexeme[]);
Token funcFL    (atlanta_chr lexeme[]);
Token funcID	(atlanta_chr lexeme[]);
Token funcSL	(atlanta_chr lexeme[]);
Token funcKEY	(atlanta_chr lexeme[]);
Token funcErr	(atlanta_chr lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcIL,		/* IL	[02] */
	NULL,		/* -    [03] */
	NULL,		/* -    [04] */
	funcFL,	    /* FL   [05] */
	NULL,		/* -    [06] */
	funcID,	    /* VMID [07] */
	NULL,		/* -    [08] */
	funcSL,		/* SL   [09] */
	funcErr,	/* ERR1 [10] */
	funcErr		/* ERR2 [11] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 21

/* TO_DO: Define the list of keywords */
static atlanta_chr* keywordTable[KWT_SIZE] = {
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
	"string"
};

#endif

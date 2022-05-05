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
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
atlanta_int line;								/* Current line number of the source code */
extern atlanta_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern atlanta_int stateType[];
extern atlanta_chr* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern atlanta_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

atlanta_int startScanner(BufferPointer psc_buf) {
	/* in case the buffer has been read previously  */
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	atlanta_chr c;	/* input symbol */
	atlanta_int state = 0;		/* initial state of the FSM */
	atlanta_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	atlanta_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	atlanta_int lexLength;		/* token length */
	atlanta_int i;				/* counter */
	atlanta_chr newc;			/* new char */
	
	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

		/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

		/* Cases for arithmethic Operators*/
		case '+':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			return currentToken;
		case '-':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			return currentToken;
		case '*':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			return currentToken;
		case '/':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			return currentToken;
		case '^':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_POW;
			return currentToken;

		/* Cases for relational Operators*/
		case '<':
			currentToken.code = REL_OP_T;
			currentToken.attribute.relationalOperator = OP_LT;
			return currentToken;
		case '>':
			currentToken.code = REL_OP_T;
			currentToken.attribute.relationalOperator = OP_GT;
			return currentToken;
		case '=':
			if (bGetChar(sourceBuffer) == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ERR_T;
			return currentToken;
		case '!':
			if (bGetChar(sourceBuffer) == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.relationalOperator = OP_NE;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = LOG_OP_T;
			currentToken.attribute.logicalOperator = OP_NOT;
			return currentToken;

		/* Case for assignment Operator*/
		case ':':
			currentToken.code = ASS_T;
			return currentToken;
			
		/* Cases for seperators */
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
		case '[':
			currentToken.code = LBK_T;
			return currentToken;
		case ']':
			currentToken.code = RBK_T;
			return currentToken;
		case ',':
			currentToken.code = COM_T;
			return currentToken;

		/* Single line Comments */
		case '#':
			newc = bGetChar(sourceBuffer);
			do {
				c = bGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					bRetract(sourceBuffer);
					/*return currentToken;*/
				} else if (c=='\n') {
					line++;
				}
			} while (c != '\n' && c!= CHARSEOF0 && c!= CHARSEOF255);
			break;		
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = bGetReadPos(sourceBuffer) - 1;
			bSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = bGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == ASWR)
				bRetract(sourceBuffer);
			lexEnd = bGetReadPos(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = bCreate((short)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			bRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
			bAddChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
			bDestroy(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

atlanta_int nextState(atlanta_int state, atlanta_chr c) {
	atlanta_int col;
	atlanta_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/*L(0), D(1), U(2), P(3), Q(4), O(5), E(6)*/

atlanta_int nextClass(atlanta_chr c) {
	atlanta_int val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3:
		val = 3;
		break;
	case CHRCOL4:
		val = 4;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 6;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 5;
	}
	return val;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(atlanta_chr lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	char lastch = lexeme[length - 1];
	int isID = ATLANTA_FALSE;

	/*call funcKey first and if doesn't find keyword, then it's vmid
	  if keyword not found, the code will be -1*/

	currentToken = funcKEY(lexeme);
	if (currentToken.attribute.codeType != -1) {
		return currentToken;
	}
	else {
		currentToken.code = MVID_T;
		isID = ATLANTA_TRUE;
	}
	
	if (isID == ATLANTA_TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VMID_LEN);
		currentToken.attribute.idLexeme[VMID_LEN] = CHARSEOF0;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */

Token funcSL(atlanta_chr lexeme[]) {
	Token currentToken = { 0 };
	atlanta_int i = 0, len = (atlanta_int)strlen(lexeme);
	currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!bAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!bAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	return currentToken;
}


/*
Function name: FuncIL
Purpose: Checks if the current token is an Integer literal
Author: Samuel Beaudoin
Version: 1
Called functions: Calls next function in automata according to the character
Parameters: the lexeme char array
Return value: returns the current token
Algorithm: Checks if the lexeme is a valid integer and sets the attributes
*/
Token funcIL(atlanta_chr lexeme[]) {
	Token currentToken = { 0 };
	atlanta_lng tlong;
	/*making sure the passed in lexeme is not empty or too big*/
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		tlong = atol(lexeme); /*converting the string to a long*/
		if (tlong >= 0 && tlong <= LLONG_MAX) {/*making sure the long is smaller than the max long long*/
			currentToken.code = IL_T;
			currentToken.attribute.intValue = (atlanta_int)tlong;
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	return currentToken;
}

/*
Function name: FuncFL
Purpose: Checks if the current token is an Float literal
Author: Samuel Beaudoin
Version: 1
Called functions: Calls next function in automata according to the character
Parameters: the lexeme char array
Return value: returns the current token
Algorithm: Checks if the lexeme is a valid float and sets the attributes
*/
Token funcFL(atlanta_chr lexeme[]) {
	Token currentToken = { 0 };
	atlanta_flt fl;
	/*making sure the passed in lexeme is not too big or empty*/
	if (lexeme[0] != '\0' && strlen(lexeme) > FNUM_LEN) {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		fl = (float)atof(lexeme);/*converting the lexeme into a float*/
		if (fl >= 0 && fl <= LLONG_MAX) {
			currentToken.code = FL_T;
			currentToken.attribute.floatValue = fl;
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(atlanta_chr lexeme[]) {
	Token currentToken = { 0 };
	atlanta_int kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
	}
	currentToken.attribute.codeType = kwindex;
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(atlanta_chr lexeme[]) {
	Token currentToken = { 0 };
	atlanta_int i = 0, len = (atlanta_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

atlanta_nul printToken(Token t) {
	extern atlanta_chr* keywordTable[]; /* link to keyword table in */

	char* arStrings[] = { "+", "-", "*", "/", "^"};
	char* relStrings[] = { "==", "!=", ">", "<"};
	char* logStrings[] = { "!" };


	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MVID_T:
		printf("MVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (short)t.attribute.codeType);
		printf("%s\n", bGetContent(stringLiteralTable,
			(short)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case LBK_T:
		printf("LBK_T\n");
		break;
	case RBK_T:
		printf("RBK_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case ASS_T:
		printf("ASS_T\n");
		break;
	case ART_OP_T:
		printf("ART_OP_T\t%s\n", arStrings[t.attribute.arithmeticOperator]);
		break;
	case REL_OP_T:
		printf("REL_OP_T\t%s\n", relStrings[t.attribute.relationalOperator]);
		break;
	case LOG_OP_T:
		printf("LOG_OP_T\t%s\n", logStrings[t.attribute.logicalOperator]);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case FL_T:
		printf("FL_T\t\t%g\n", t.attribute.floatValue);
		break;
	case IL_T:
		printf("IL_T\t\t%d\n", t.attribute.intValue);
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

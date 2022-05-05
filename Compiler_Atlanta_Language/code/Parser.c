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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TODO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TODO: This is the function to start the parser - check your program definition */

atlanta_nul startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		optionalStatements();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TODO: This is the main code for match - check your definition */
atlanta_nul matchToken(atlanta_int tokenCode, atlanta_int tokenAttribute) {
	atlanta_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		matchFlag = 1;
		break;
	case MVID_T:
		matchFlag = 1;
		break;
	case ASS_T:
		matchFlag = 1;
		break;
	case EOS_T:
		matchFlag = 1;
		break;
	case LPR_T:
		matchFlag = 1;
		break;
	case RPR_T:
		matchFlag = 1;
		break;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		//syncErrorHandler(tokenCode);
		printf("%d", lookahead.code);
		
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TODO: This is the function to handler error - adjust basically datatypes */
atlanta_nul syncErrorHandler(atlanta_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TODO: This is the function to error printing - adjust basically datatypes */
atlanta_nul printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MVID_T:
		printf("%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}



/*
Function name: optionalStatements
Purpose: Function that checks for optional statements
Author: Samuel Beaudoin
Version: 1
Called functions: calls statements if statements are detected
Parameters: NA
Return value: nothing
*/
atlanta_nul optionalStatements() {
	switch (lookahead.code) {
	case MVID_T:
		statements();
		break;
	case KW_T:
		statements();
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
Function name: statements
Purpose: Function that checks for statements
Author: Samuel Beaudoin
Version: 1
Called functions: calls statement and statementsPrime to avoid recursion problems
Parameters: NA
Return value: nothing
*/
atlanta_nul statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
Function name: statements
Purpose: Function that checks for ID's and keywords to spot the start fo a new statement
Author: Samuel Beaudoin
Version: 1
Called functions: calls statements
Parameters: NA
Return value: nothing
*/
atlanta_nul statementsPrime() {
	switch (lookahead.code) {
	case MVID_T:
	case KW_T:
		statements();
	default:
		; //empty string
	}
}

/*
Function name: statement
Purpose: Function that checks for the different types of statements
Author: Samuel Beaudoin
Version: 1
Called functions: calls assignmentStatement for assignment statements and output statement for output statements
Parameters: NA
Return value: nothing
*/
atlanta_nul statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case DOUBLE:
		case INT:
		case STRING:
			matchToken(KW_T, NO_ATTR);
			assignmentStatement();
			break;
		default:
			printError();
		}
		break;
	case MVID_T:
		if (strncmp(lookahead.attribute.idLexeme, "show", 4) == 0) {
			output_statement();
		}
		else {
			assignmentStatement();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
Function name: assignmentStatement
Purpose: Function that checks for assignent statement
Author: Samuel Beaudoin
Version: 1
Called functions: calls assignmentExpression
Parameters: NA
Return value: nothing
*/
atlanta_nul assignmentStatement() {
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


/*
Function name: assignmentExpression
Purpose: Function that checks for assignment expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls arithmatic_expression, string_expression, function_expression, printError
Parameters: NA
Return value: nothing
*/
atlanta_nul assignmentExpression() {
	switch (lookahead.code) {
	case MVID_T:
		matchToken(MVID_T, NO_ATTR);
		matchToken(ASS_T, NO_ATTR);
		switch (lookahead.code) {
		case FL_T:
		case IL_T:
			arithmatic_expression();
			break;
		case STR_T:
			string_expression();
			break;
		case MVID_T:
			function_expression();
			break;
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}


/*
Function name: arithmatic_expression
Purpose: Function that checks for arithmetic expressions
Author: Samuel Beaudoin
Version: 1
Called functions: calls unary_arithmatic_expression, additive_arithmatic_expression, printError
Parameters: NA
Return value: nothing
*/
atlanta_nul arithmatic_expression() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_MUL:
		case OP_DIV:
		case OP_POW:
			printError(); /* illegal match */
			return;
		default:
			break;
		}
		unary_arithmatic_expression();
		break;

	case MVID_T:
	case FL_T:
	case IL_T:
	case LPR_T:
		additive_arithmatic_expression();
		break;

	default:
		printError();
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithmatic expression parsed");
}



/*
Function name: unary_arithmatic_expression
Purpose: Function that checks for unary arithmetic expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls primary_arithmatic_expression, printError
Parameters: NA
Return value: nothing
*/
atlanta_nul unary_arithmatic_expression() {
	switch (lookahead.code)
	{
	case ART_OP_T:
		switch (lookahead.attribute.arithmeticOperator)
		{
		case OP_MUL:
		case OP_DIV:
			printError();
			return;
		default:
			break;
		}
		matchToken(lookahead.code, lookahead.attribute.arithmeticOperator);
		primary_arithmatic_expression();
		printf("%s%s\n", STR_LANGNAME, ": Unary arithmatic expression parsed");
		break;
	default:	/* no match */
		printError();
		return;
	}
}


/*
Function name: primary_arithmatic_expression
Purpose: Function that checks for primary arithmatic expressions
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken, arithmatic_expression, printError
Parameters: NA
Return value: nothing
*/
atlanta_nul primary_arithmatic_expression() {
	switch (lookahead.code) {
	case MVID_T:
		matchToken(MVID_T, NO_ATTR);
		break;
	case IL_T:
		matchToken(IL_T, NO_ATTR);
		break;
	case FL_T:
		matchToken(FL_T, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmatic_expression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		printError();
		return;
	}
	
	printf("%s%s\n", STR_LANGNAME, ": Primary arithmatic expression parsed");
}


/*
Function name: additive_arithmatic_expression
Purpose: Function that checks for additive arithmatic expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls multiplicative_arithmatic_expression, additive_arithmatic_expression_prime
Parameters: NA
Return value: nothing
*/
atlanta_nul additive_arithmatic_expression() {
	multiplicative_arithmatic_expression();
	additive_arithmatic_expression_prime();
}


/*
Function name: multiplicative_arithmatic_expression
Purpose: Function that checks for multiplicative arithmatic expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls primary_arithmatic_expression, multiplicative_arithmatic_expression_prime
Parameters: NA
Return value: nothing
*/
atlanta_nul multiplicative_arithmatic_expression() {
	primary_arithmatic_expression();
	multiplicative_arithmatic_expression_prime();
}


/*
Function name: multiplicative_arithmatic_expression_prime
Purpose: Prime function for multiplicative arithmatic expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls primary_arithmatic_expression, multiplicative_arithmatic_expression_prime
Parameters: NA
Return value: nothing
*/
atlanta_nul multiplicative_arithmatic_expression_prime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_MUL:
			matchToken(ART_OP_T, OP_MUL);
			primary_arithmatic_expression();
			multiplicative_arithmatic_expression_prime();
			break;
		case OP_DIV:
			matchToken(ART_OP_T, OP_DIV);
			primary_arithmatic_expression();
			multiplicative_arithmatic_expression_prime();
			break;
		default:
			return;
		}
		break;
	default:
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Multiplicative arithmatic expression parsed");
}



/*
Function name: additive_arithmatic_expression_prime
Purpose: Prime function for multiplicative arithmatic expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken, multiplicative_arithmatic_expression,  additive_arithmatic_expression_prime
Parameters: NA
Return value: nothing
*/
atlanta_nul additive_arithmatic_expression_prime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_ADD:
			matchToken(ART_OP_T, OP_ADD);
			multiplicative_arithmatic_expression();
			additive_arithmatic_expression_prime();
			break;
		case OP_SUB:
			matchToken(ART_OP_T, OP_SUB);
			multiplicative_arithmatic_expression();
			additive_arithmatic_expression_prime();
			break;

		default:
			return;
		}
		break;
	default:
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmatic expression parsed");
}



/*
Function name: variable_list
Purpose: Function that checks for variable lists
Author: Samuel Beaudoin
Version: 1
Called functions: calls variable_list_prime, variable_identifier
Parameters: NA
Return value: nothing
*/
atlanta_nul variable_list() {
	variable_identifier();
	variable_list_prime();
	printf("%s%s\n", STR_LANGNAME, ": Variable list parsed");
}



/*
Function name: variable_identifier
Purpose: Function that checks for variable lists identifiers
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken, printError
Parameters: NA
Return value: nothing
*/
atlanta_nul variable_identifier() {
	switch (lookahead.code) {
	case MVID_T:
		matchToken(MVID_T, NO_ATTR);
		break;
	default:
		printError();
		return;
	}
}




/*
Function name: variable_list_prime
Purpose: Prime Function for for variable lists identifiers
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken, variable_identifier, variable_list_prime
Parameters: NA
Return value: nothing
*/
atlanta_nul variable_list_prime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		variable_identifier();
		variable_list_prime();
		break;

	default:
		break;
	}
}



/*
Function name: output_statement
Purpose: Prime Function for for output statement
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken, output_list
Parameters: NA
Return value: nothing
*/
atlanta_nul output_statement() {
	matchToken(MVID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	output_list();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}



/*
Function name: output_list
Purpose: Prime Function for for output list
Author: Samuel Beaudoin
Version: 1
Called functions: calls variable_list_prime, matchToken, 
Parameters: NA
Return value: nothing
*/
atlanta_nul output_list() {
	switch (lookahead.code) {
	case MVID_T:
		variable_list_prime();
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		variable_list_prime();
		printf("%s%s\n", STR_LANGNAME, ": Output list (string literal) parsed");
		break;

	default:
		printError();
		break;
	}
}



/*
Function name: string_expression
Purpose: Prime Function for string expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls primary_string_expression, string_expression_prime,
Parameters: NA
Return value: nothing
*/
atlanta_nul string_expression() {
	primary_string_expression();
	string_expression_prime();
}



/*
Function name: primary_string_expression
Purpose: Prime Function for primary string expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken
Parameters: NA
Return value: nothing
*/
atlanta_nul primary_string_expression() {
	switch (lookahead.code) {
	case MVID_T:
		matchToken(MVID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary string expression parsed");
}



/*
Function name: string_expression_prime
Purpose: Prime Function for primary string expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken, primary_string_expression, string_expression_prime
Parameters: NA
Return value: nothing
*/
atlanta_nul string_expression_prime() {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		primary_string_expression();
		string_expression_prime();
		break;
	default:
		printf("%s%s\n", STR_LANGNAME, ": String expression parsed");
	}
}



/*
Function name: function_expression
Purpose: Prime Function for primary string expression
Author: Samuel Beaudoin
Version: 1
Called functions: calls matchToken
Parameters: NA
Return value: nothing
*/
atlanta_nul function_expression() {
	matchToken(MVID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	matchToken(STR_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);

	printf("%s%s\n", STR_LANGNAME, ": Function expression parsed");
}


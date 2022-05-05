/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa - Samuel Beaudoin
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: compilers.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa - Samuel Beaudoin
* Course: CST 8152 – Compilers, Lab Section: 013
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define ATLANTA_TRUE	1
#define ATLANTA_FALSE 0

/* You should add your own constant definitions here */
#define ATLANTA_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */ 

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/
typedef char			atlanta_chr;
typedef short			atlanta_int;
typedef float			atlanta_flt;
typedef void			atlanta_nul;

typedef unsigned char	atlanta_bol;
typedef unsigned char	atlanta_flg;

typedef long			atlanta_lng;
typedef double			atlanta_dbl;

/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER	= '1',
	PGM_SCANNER = '2',
	PGM_PARSER	= '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
atlanta_int mainBuffer	(atlanta_int argc, atlanta_chr** argv);
atlanta_int mainScanner	(atlanta_int argc, atlanta_chr** argv);
atlanta_int mainParser	(atlanta_int argc, atlanta_chr** argv);
#endif

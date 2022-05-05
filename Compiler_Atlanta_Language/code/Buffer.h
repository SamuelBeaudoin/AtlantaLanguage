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
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};

/* TO_DO: Adjust all constants to your Language */

#define BUFFER_ERROR (-1)						/* General error message */
#define BUFFER_EOF '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (atlanta) .................................. */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	100			/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for atlanta */
/* BITS                                (7654.3210) */
#define atlanta_DEFAULT_FLAG 0x0F 	/* (0000.1111)_2 = (015)_10 */
/* TO_DO: BIT 7: REL = Relocation */
#define atlanta_SET_FLAG_RLB 0x80		/* (1000.0000)_2 = (128)_10 */
#define atlanta_RST_FLAG_RLB 0x7F		/* (0111.1111)_2 = (127)_10 */
#define atlanta_CHK_FLAG_RLB 0x80		/* (1000.0000)_2 = (128)_10 */
/* TO_DO: BIT 6: EOB = EndOfBuffer */
#define atlanta_SET_FLAG_EOB 0x40		/* (0100.0000)_2 = (064)_10 */
#define atlanta_RST_FLAG_EOB 0xBF		/* (1011.1111)_2 = (191)_10 */
#define atlanta_CHK_FLAG_EOB 0x40		/* (0100.0000)_2 = (064)_10 */
/* TO_DO: BIT 5: FUL = Full */
#define atlanta_SET_FLAG_FUL 0x20		/* (0010.0000)_2 = (032)_10 */
#define atlanta_RST_FLAG_FUL 0xDF		/* (1111.1101)_2 = (223)_10 */
#define atlanta_CHK_FLAG_FUL 0x20		/* (0010.0000)_2 = (032)_10 */
/* TO_DO: BIT 4: EMP: Empty */
#define atlanta_SET_FLAG_EMP 0x10		/* (0001.0000)_2 = (016)_10 - OR  */
#define atlanta_RST_FLAG_EMP 0xEF		/* (1110.1111)_2 = (239)_10 - AND */
#define atlanta_CHK_FLAG_EMP 0x10		/* (0001.0000)_2 = (016)_10 - AND */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (atlanta) .................................. */

/* TO_DO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	atlanta_int markPos;			/* the offset (in chars) to the mark location */
	atlanta_int readPos;			/* the offset (in chars) to the get-character location */
	atlanta_int writePos;			/* the offset (in chars) to the add-character location */
} Position;

/* Buffer structure */
typedef struct buffer {
	atlanta_chr* string;		/* pointer to the beginning of character array (character buffer) */
	atlanta_int	size;		    /* current dynamic memory size (in bytes) allocated to character buffer */
	atlanta_int	increment;		/* character array increment factor */
	atlanta_int	mode;			/* operational mode indicator */
	atlanta_flg	flags;			/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bCreate(atlanta_int, atlanta_int, atlanta_int);
BufferPointer	bAddChar(BufferPointer const, atlanta_chr);
atlanta_bol		bRetract(BufferPointer const);
atlanta_bol		bRestore(BufferPointer const);
atlanta_bol		bRecover(BufferPointer const);
atlanta_bol		bClear(BufferPointer const);
atlanta_bol		bDestroy(BufferPointer const);
atlanta_bol		bSetMark(BufferPointer const, atlanta_int);
atlanta_bol		bIsEmpty(BufferPointer const);
atlanta_bol		bIsFull(BufferPointer const);
atlanta_bol		bIsRealocated(BufferPointer const);
atlanta_int		bPrint(BufferPointer const);
atlanta_int		bLoad(BufferPointer const, FILE* const);
/* Getters */
atlanta_int		bGetSize(BufferPointer const);
atlanta_int		bGetWritePos(BufferPointer const);
atlanta_int		bGetMarkPos(BufferPointer const);
atlanta_int		bGetReadPos(BufferPointer const);
atlanta_int		bGetIncrement(BufferPointer const);
atlanta_int		bGetMode(BufferPointer const);
atlanta_chr		bGetChar(BufferPointer const);
atlanta_chr* bGetContent(BufferPointer const, atlanta_int);
atlanta_flg		bGetFlags(BufferPointer const);

#endif

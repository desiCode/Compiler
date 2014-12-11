/************************************************************************************************************************************
File name: stable.h
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 2
Date: Oct 25, 2013 
Professor: Sv. Ranev
Purpose: Declarations of the defines 

************************************************************************************************************************************/

#ifndef  STABLE_H_
#define  STABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif
/***********************user****defines*******************************************************************/
/* MASKS */
/* 16 bit field MSB->  15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 <-LSB */
#define INIL_VAL	0x0000  /* 0000 0000 0000 0000 */
#define DEF_VAL		0xFFF8  /* 1111 1111 1111 1000 */
#define RESET_TYPE  0xFFF9  /* 1111 1111 1111 1001 */
#define ST_INT		0x0002  /* 0000 0000 0000 0010 */
#define ST_FLT		0x0004  /* 00b00 0000 0000 0100 */
#define ST_SRT		0x0006  /* 0000 0000 0000 0110 */
#define ST_UFLG		0x0001  /* 0000 0000 0000 0001 */
#define CHK_UFLG    0x0001  /* 0000 0000 0000 0001 */

#define INT_CAP		1000	/*intial capacity of the buffer */ 
#define INT_INC		100		/*intial increment value of the buffer*/
#define OUTFILE		"$stable.ste" /*name of file where output will be stored */
#define FAIL_A		-1		/**/
#define F_FAIL		-2		/*if any function failed*/
#define TW_O	2
#define FOU_R	4
#define SI_X	6
/*********************************************************************************************************/

typedef union InitialValue{
	int int_val;	/*integer variable initial value*/
	float fpl_val;	/*floating-point variable intial value*/
	int str_offset; /*string variable initial value (offset)*/
}InitialValue;

typedef struct SymbolTableVidRecord{
	unsigned short status_field;/*vairable record status field*/
	char * plex; /* pointer to lexeme (VID name) in CA*/
	int o_line; /*line of first occurrence*/
	InitialValue i_value;
	size_t ds_offset; /*offset from the beginning of data segment*/
}STVR;

typedef struct SymbolTableDescriptor{
	STVR *pstvr;	/*pointer to aray of STVR*/
	int st_size;	/*size in number of STVR elements */
	int st_offset;  /*offset in number of STVR elements*/
	Buffer * plsBD;	/*pointer to the lexeme storage buffer descriptor*/
}STD;

STD st_create(int st_size);
int st_install(STD sym_table, char *lexeme , int line);
int st_lookup(STD sym_table, char * lexeme);
int st_update_type(STD sym_table, int vid_offset,char v_type);
int st_update_value(STD sym_table , int vid_offset,InitialValue i_value);
char st_get_type(STD sym_table,int vid_offset);
void st_destroy(STD sym_table);
int st_print(STD sym_tabe);
static void st_setsize(void);
static void st_incoffset(void);
int st_store(STD sym_table);
int st_sort(STD sym_table,char s_order);
static int compare_d(const void *a, const void *b);
static int compare_a(const void *a, const void *b);
#endif
                     
/*********************************************************************XXXX*****************************************************************************************/

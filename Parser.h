/************************************************************************************************************************************
File name: parser.h
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 4
Date: Dec 6, 2013 
Professor: Sv. Ranev
Purpose: Declarations of the defines used in parser.c

************************************************************************************************************************************/

#ifndef PARSE_H
#define PARSE_H

/***********************user****defines*************************************************************************/
#ifndef BUFFER_H_
#include "buffer.h"
#endif
#ifndef TOKEN_H_
#include "token.h"
#endif
#ifndef  STABLE_H_
#include "stable.h"
#endif


#define NO_ATTR	-1
/********************** Variables ******************************************************************************/
static Token lookahead_token;
static Buffer * sc_buf;
int synerrno;

extern char * kw_table[];		/*key word table defined in table.h*/
extern int line;				
extern STD sym_table;    
extern Buffer * str_LTBL; 

extern Token  mlwpar_next_token(Buffer *);
enum {
ELSE,
IF,
INPUT,
OUTPUT,
PLATYPUS,
REPEAT,
THEN,
USING		
};
/*********************** function prototypes *******************************************************************/
void parser(Buffer * in_buf);
void match(int pr_token_code, int pr_token_attribute);
void syn_eh(int sync_token_code);
void syn_printe(void);
void program(void);
void opt_statements(void);
void statements(void);
void statement_p(void);
void statement(void);
void assignment_statement(void);
void assignment_expression(void);
void arithemetic_expression(void);
void string_expression(void);
void unary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void additive_arithmetic_expression_p(void);
void multiplicative_arith_expression(void);
void multiplicative_arith_expression_p(void);
void conditional_expression(void);
void logical_or_expression(void);
void logical_or_expression_p(void);
void logical_and_expression(void);
void logical_and_expression_p(void);
void relational_expression(void);
void relational_operator(void);
void variable_list(void);
void variable_list_p(void);
void variable_identifier(void);
void output_list(void);
void selection_statement(void);
void iteration_statement(void);
void input_statement(void);
void output_statement(void);
void primary_string_expression(void);
void primary_string_expression_p(void);
void primary_a_relational_expression(void);
void primary_s_relational_expression(void);
void primary_arithmetic_expression(void);
#endif

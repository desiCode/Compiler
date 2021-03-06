/************************************************************************************************************************************
File name: parser.c
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 4
Date: Dec 6, 2013 
Professor: Sv. Ranev
Version/History : The number tells you how many times the function has been edited !
Purpose:	The purpose of the parser obtains a string of tokens from the lexical
			analyzer and verifies that the string of token names can be generated by the grammar for the source language. We expect the
			parser to report any syntax errors in an intelligible fashion

Function List:
void parser(Buffer * in_buf);
void match(int pr_token_code, int pr_token_attribute);
void syn_eh(int sync_token_code);
void syn_printe(void);void program(void);void opt_statements(void);
void statements(void);void statement_p(void);void statement(void);
void assignment_statement(void);void assignment_expression(void);void arithemetic_expression(void);
void string_expression(void);void unary_arithmetic_expression(void);void additive_arithmetic_expression(void);
void additive_arithmetic_expression_p(void);void multiplicative_arith_expression(void);void multiplicative_arith_expression_p(void);
void conditional_expression(void);void logical_or_expression(void);void logical_or_expression_p(void);
void logical_and_expression(void);void logical_and_expression_p(void);void relational_expression(void);
void relational_operator(void);void variable_list(void);void variable_list_p(void);
void variable_identifier(void);void output_list(void);void selection_statement(void);
void iteration_statement(void);void input_statement(void);void output_statement(void);
void primary_string_expression(void);void primary_string_expression_p(void);void primary_a_relational_expression(void);
void primary_s_relational_expression(void);void primary_arithmetic_expression(void);

********************************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
/*********************** includes *************************************************************************/
#include "parser.h"
#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
/*********************** user **** defines *************************************************************************/

#define gen_incode(SExp)	printf("%s\n",SExp);	/*macro of the function gen_incode*/
/*******************************************************************************************************************************************************
Purpose: the function takes a token and its argument as parameter and tries to match it with the token recieved from Scanner
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: syn_printe() syn_eh();
Parameters: pr_token_code = token code 
			pr_token_attribute = token attribute
Return value: 
Algorithm: take token code match it to token_code expexted accoring to the grammar math attribute only in certain cases
********************************************************************************************************************************************************/
void match(int pr_token_code, int pr_token_attribute){
	printf("|%d| to match |%d|\t",pr_token_code,pr_token_attribute);
	printf("|%d| to match |%d|\n",lookahead_token.code,lookahead_token.attribute.get_int);
	if(lookahead_token.code == pr_token_code){
	switch(pr_token_code) {
	case SEOF_T:
		return;
	case KW_T:
	case LOG_OP_T:
	case ART_OP_T:
	case REL_OP_T:
			if(lookahead_token.attribute.get_int != pr_token_attribute)
				break;	/*halt we have a problem*/
	default:
		lookahead_token= mlwpar_next_token(sc_buf);
		if(lookahead_token.code ==ERR_T){
			syn_printe();
			lookahead_token= mlwpar_next_token(sc_buf);
			++synerrno;
			return;
		}
		return;
	}
	}
	 syn_eh(pr_token_code);
	 return;
}
/*******************************************************************************************************************************************************
Purpose: the function acts as a panic method that stop the parser from panic when received a wrong token it searches for the token in the whole file
			if the token in not found before end of file it exits with and error number;
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: exit() syn_printe()
Parameters: 
Return value: void
Algorithm: 
********************************************************************************************************************************************************/
void syn_eh(int sync_token_code){
	syn_printe();
	++synerrno;
	while(lookahead_token.code != sync_token_code){
		 
		 if(sync_token_code != SEOF_T && lookahead_token.code == SEOF_T)
			 exit(synerrno);
		 
		 lookahead_token= mlwpar_next_token(sc_buf);
	}
	if(lookahead_token.code != SEOF_T ){
			 lookahead_token = mlwpar_next_token(sc_buf);
		 }
	return ;
}
/*******************************************************************************************************************************************************
Purpose: the purpose of the function is to print token code and attribute when it does not matches 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: none
Parameters: void
Return value: void

********************************************************************************************************************************************************/
void syn_printe(void){
	Token token = lookahead_token;
	printf("PLATY: Syntax error:  Line:%3d\n",line);
	printf("*****  Token code:%3d Attribute: ",token.code);
	switch(token.code){
	case LPR_T:
	case RBR_T:
	case COM_T:
	case EOS_T:
	case SCC_OP_T:
	case SEOF_T:
	case LBR_T:
	case RPR_T:
		printf("NA\n");
		break;
	case KW_T:
		printf("%s\n",kw_table[token.attribute.get_int]);
		break;
	case ERR_T:
		printf("%s\n",token.attribute.err_lex);
		break;
	case ART_OP_T:
	case REL_OP_T:
	case LOG_OP_T:
		printf("%d\n",token.attribute.get_int);
		break;
	case FPL_T:
		printf("%5.1f\n",token.attribute.flt_value);
		break;
	case AVID_T:
	case SVID_T:
		printf("%s\n",sym_table.pstvr[token.attribute.get_int].plex);
		break;
	case INL_T:
		printf("%d\n",token.attribute.get_int);
		break;
	case STR_T:
		printf("%s\n",b_get_chmemloc(str_LTBL,token.attribute.get_int));
		break;
	default:
		printf("Invalid Token Code %d\n",token.code);
		break;
	}
	return;
}

/***************************************************************************************************************/
void parser(Buffer * in_buf){
	if(in_buf !=NULL ){
sc_buf = in_buf;/* check for validity */
lookahead_token= mlwpar_next_token(sc_buf);
 program(); match(SEOF_T,NO_ATTR);
gen_incode("PLATY: Source file parsed");
	}
	else 
	return ;
}
/***************************************************************************************************************/
void program(void){
match(KW_T,PLATYPUS);	
match(LBR_T,NO_ATTR); opt_statements();	match(RBR_T,NO_ATTR);
gen_incode("PLATY: Program parsed");
}
/***************************************************************************************************************/
void opt_statements(void){
	/*First set:{ AVID,SVID,IF,USING,INPUT,OUTPUT}*/
	switch(lookahead_token.code){
	case AVID_T:
	case SVID_T:	statements();  break;
	case KW_T:
		/*check ofr PLATYPUS ,ELSE , THEN ,REPEAT here and in statements_p()*/
		if(lookahead_token.attribute.get_int != PLATYPUS &&
			lookahead_token.attribute.get_int != ELSE &&
			lookahead_token.attribute.get_int != THEN &&
			lookahead_token.attribute.get_int != REPEAT ) {
				statements();
				return;
		}
	default:
		gen_incode("PLATY: Opt_statements parsed");
	}
}
/***************************************************************************************************************
*<statements> ->
			<statement> <statement ' >
* FirstSet(statements)={AVID,SVID,IF,USING,INPUT,OUTPUT};
*
***************************************************************************************************************/
void statements(void){
		statement();
		statement_p();
}
/***************************************************************************************************************
*First(statement ' ) = {AVID,SVID,IF,USING,INPUT,OUTPUT ,e)
<statement '>-> 
				<statement><statement '> | e ;
*
*
***************************************************************************************************************/
void statement_p(void){

	switch(lookahead_token.code){
	case KW_T:
		switch(lookahead_token.attribute.get_int){
		case ELSE:
		case REPEAT:
		case THEN:
		case PLATYPUS:
			return;		/*break out of this inner switch*/
		default:
			break;
		}
	case AVID_T:
	case SVID_T:
		statement();
		statement_p();
		break;
	default:
	
		break;
	}
}
/***************************************************************************************************************
*First(statement)=( {AVID,SVID,IF,USING,INPUT,OUTPUT)

*<statement> ->
		  <assignment statement>
		| <selection statement>
		| <iteration statement>
 		| <input statement>
		| <output statement>
*
*
***************************************************************************************************************/
void statement(void){
	switch(lookahead_token.code){

	case AVID_T:
	case SVID_T:
		assignment_statement();
		break;
	case KW_T:
		switch(lookahead_token.attribute.kwt_idx){
		case IF:
			selection_statement();
			break;
		case USING:
			iteration_statement();

			break;
		case INPUT:
			input_statement();
			break;
		case OUTPUT:
			output_statement();
			break;
		default:
			syn_printe();
			break;
		}
		break;
	default:
		syn_printe();	/*new code*/
		break;
	}
}
/***************************************************************************************************************
*First(assignment statement)={AVID,SVID}
<assignment statement> -> 
	<assignment expression>;
*
***************************************************************************************************************/
void assignment_statement(void){
	switch(lookahead_token.code){
	case AVID_T:
	case SVID_T:
	assignment_expression(); match(EOS_T,NO_ATTR);
	
	break;
	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Assignment statement parsed");
}
/***************************************************************************************************************
*First(assignment expression)= {AVID,SVID}
< assignment expression> -> 
		  AVID = <arithmetic expression>
		| SVID = <string expression>
*
***************************************************************************************************************/
void assignment_expression(void){
	switch(lookahead_token.code){
	case AVID_T:
		match(AVID_T,NO_ATTR);
		match(ASS_OP_T,NO_ATTR);
		arithemetic_expression();
		gen_incode("PLATY: Assignment expression (arithmetic) parsed");
		break;
	case SVID_T:
		match(SVID_T,NO_ATTR);
		match(ASS_OP_T,NO_ATTR);
		string_expression();
		gen_incode("PLATY: Assignment expression (string) parsed");
		break;
	default:
		syn_printe();
		return;
	}
}
/***************************************************************************************************************
*
FIRST(arithmetic expression )={ - ,+,AVID,FPL,IL,(}
<arithmetic expression> - >
		  <unary arithmetic expression>  
		| <additive arithmetic expression>	
*
*
***************************************************************************************************************/
void arithemetic_expression(void){
	switch(lookahead_token.code){
	case ART_OP_T:
		switch(lookahead_token.attribute.arr_op){
		case MULT:
		case DIV:
			/*syn_printe();*/
			return;
		default:
			break; /*MINUS PLUS*/
		}	
		unary_arithmetic_expression();
		break;
	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T:
		additive_arithmetic_expression();
		break;
	default:
		
		return;
	}
	gen_incode("PLATY: Arithmetic expression parsed");

}
/**************************************************************************************************************
*FIRST(unary arithmetic expression )={-,+}
<unary arithmetic expression> ->
			  -  <primary arithmetic expression> 
			| + <primary arithmetic expression>
*
***************************************************************************************************************/
void unary_arithmetic_expression(void){
	if(lookahead_token.code == ART_OP_T){
		switch(lookahead_token.attribute.arr_op){
		case MULT:
		case DIV:
			/*syn_printe();*/
			return;
		default:
			match(lookahead_token.code,lookahead_token.attribute.arr_op);
			primary_arithmetic_expression();
			
			break;
		}
	}
	else {
	/*syn_printe();*/
	}
	gen_incode("PLATY: Unary arithmetic expression parsed");
}
/***************************************************************************************************************
*
FIRST(additive arithmetic expression) ={AVID,FPL,IL}
<additive arithmetic expression> ->		
				 <multiplicative arithmetic expression> <additive arithmetic expression ' > 

*
***************************************************************************************************************/
void additive_arithmetic_expression(void){
		multiplicative_arith_expression(); additive_arithmetic_expression_p();
}
/***************************************************************************************************************
*
FIRST()={+,-,e}
<additive arithmetic expression ' > 	
					+ <multiplicative arithmetic expression>  <additive arithmetic expression ' > 
					| - <multiplicative arithmetic expression> <additive arithmetic expression ' > 
					| e
*
***************************************************************************************************************/
void additive_arithmetic_expression_p(void){
	switch(lookahead_token.code){
	case ART_OP_T:
		switch(lookahead_token.attribute.arr_op){
		case MINUS:
			match(ART_OP_T,MINUS);
			multiplicative_arith_expression(); additive_arithmetic_expression_p();
			gen_incode("PLATY: Additive arithmetic expression parsed");
			break;
		case PLUS:
			match(ART_OP_T,PLUS);
			multiplicative_arith_expression(); additive_arithmetic_expression_p();
			gen_incode("PLATY: Additive arithmetic expression parsed");
			break;
		}
	default:
		break;
	}
	
}
/*************************************************************************************************************
*
FIRST(multiplicative arithmetic expression )={*,/,AVID,FPL,IL,(}
<multiplicative arithmetic expression> ->
			<primary arithmetic expression> <multiplicative arithmetic expression '> 
*
***************************************************************************************************************/
void multiplicative_arith_expression(void){
	
		primary_arithmetic_expression(); multiplicative_arith_expression_p();
		
}
/*************************************************************************************************************
*FIRST(multiplicative arithmetic expression )={*,/,}
<multiplicative arithmetic expression ' >	 
			*<primary arithmetic expression> <multiplicative arithmetic expression'>			
			|/<primary arithmetic expression><multiplicative arithmetic expression'>			
			| e
*
***************************************************************************************************************/
void multiplicative_arith_expression_p(void){
	switch(lookahead_token.code){
	case ART_OP_T:
		switch(lookahead_token.attribute.arr_op){
		case PLUS:
		case MINUS:
			return;
		default:
			break; /*DIV MULT*/
		}
			match(lookahead_token.code,lookahead_token.attribute.arr_op);				
			primary_arithmetic_expression(); multiplicative_arith_expression_p();
		    gen_incode("PLATY: Multiplicative arithmetic expression parsed");
			break;
	}
}

/***************************************************************************************************************
*
First <string expression>= {SVID_T,STR_T,<>}
<string expression> ->
<primary string expression><string expressioní>
*
***************************************************************************************************************/
void string_expression(void){
	primary_string_expression(); primary_string_expression_p();
	gen_incode("PLATY: String expression parsed");
}
/***************************************************************************************************************
*
First <string expressioní>= {<>,E}
<string expressioní>->
		| <> <primary string expression> <primary string expression'>
		|e

*
***************************************************************************************************************/
void primary_string_expression_p(void){
	switch(lookahead_token.code){
	case SCC_OP_T:
		match(SCC_OP_T,NO_ATTR); primary_string_expression(); primary_string_expression_p();
			break;
	default:
		break;
	}
	
}
/***************************************************************************************************************
*
First(selection statement )= {IF};
<selection statement> ->
  IF (<conditional expression> )  THEN  <opt_statements> 
  ELSE { <opt_statements> } ;
*
***************************************************************************************************************/
void selection_statement(void){
	
			match(KW_T,IF);
			match(LPR_T,NO_ATTR);
			conditional_expression();
			match(RPR_T,NO_ATTR);	match(KW_T,THEN);
			opt_statements();
			match(KW_T,ELSE);
			match(LBR_T,NO_ATTR);	
			opt_statements();
			match(RBR_T,NO_ATTR);
			match(EOS_T,NO_ATTR);
			gen_incode("PLATY: IF statement parsed");
}
/***************************************************************************************************************
*
First(iteration statement) = {USING}

<iteration statement> ->
             USING <assignment expression>,
	<conditional expression>,<assignment expression>)
REPEAT(opt_statements);};
*
***************************************************************************************************************/
void iteration_statement(void){
	
			match(KW_T,USING);match(LPR_T,NO_ATTR);
			assignment_expression();match(COM_T,NO_ATTR);

			conditional_expression();
			match(COM_T,NO_ATTR);
			assignment_expression();match(RPR_T,NO_ATTR);
			
			match(KW_T,REPEAT); match(LBR_T,NO_ATTR);
			
			opt_statements(); 
			
			
			match(RBR_T,NO_ATTR);
			match(EOS_T,NO_ATTR);
		
			gen_incode("PLATY: USING statement parsed");
}
/***************************************************************************************************************
*
FIRST (conditional expression)= {FPL,IL,AVID,SL,SVID}
<conditional expression> ->
				<logical OR expression>
*
***************************************************************************************************************/
void conditional_expression(void){
		logical_or_expression();
		gen_incode("PLATY: Conditional expression parsed");
	
}
/***************************************************************************************************************
*
First(logical OR expression )={FPL,IL,AVID,SL,SVID}
<logical OR expression> ->
			 <logical AND expression>  <logical OR expression ' > 
*
***************************************************************************************************************/
void logical_or_expression(void){
	logical_and_expression(); logical_or_expression_p();
}

/***************************************************************************************************************
*
First(logical OR expression ')={.OR.,e}
<logical OR expression ' > ->
 				.OR. <logical AND expression> <logical OR expression ' > 
				| e 
*
***************************************************************************************************************/
void logical_or_expression_p(){

	if(lookahead_token.code == LOG_OP_T){
		switch(lookahead_token.attribute.log_op){
		case OR:						/*new code*/
		match(LOG_OP_T,OR);
		logical_and_expression(); logical_or_expression_p();
		gen_incode("PLATY: Logical OR expression parsed");	
		break;
		default:
			return;
		}
	}
}
/***************************************************************************************************************
*
First(logical AND expression )={FPL,IL,VID,SL,SVID}
<logical AND expression> ->
				<relational expression> <logical AND expression '>
*
***************************************************************************************************************/
void logical_and_expression(){
			relational_expression(); logical_and_expression_p();			
}

/***************************************************************************************************************
*
First(logical AND expression ' ) = { .AND. ,e}
 <logical AND expression '>->
				 	.AND. <relational expression><logical AND expression '>
					| e 
*
***************************************************************************************************************/
void logical_and_expression_p(void){
	if(lookahead_token.code == LOG_OP_T){
		switch(lookahead_token.attribute.log_op){
		case AND:									/*new code*/
			match(LOG_OP_T,AND);
			relational_expression();logical_and_expression_p();	
			gen_incode("PLATY: Logical AND expression parsed");
			break;
		default:
			return;
		}
	}
}
/***************************************************************************************************************
*
FIRST(relational expression)={FPL,INL_T,AVID,SVID,STR_T};
<relational expression> ->
<primary a_relational expression>  <relational operator>  <primary a_relational expression> |	
 <primary s_relational expression>  <relational operator>  <primary s_relational expression>

*
***************************************************************************************************************/
void relational_expression(void){
	switch(lookahead_token.code){
	case FPL_T:
	case INL_T:
	case AVID_T:
		primary_a_relational_expression(); relational_operator(); primary_a_relational_expression();
		break;
	case STR_T:
	case SVID_T:
		primary_s_relational_expression(); relational_operator(); primary_s_relational_expression();
		break;
	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Relational expression parsed");
}
/***************************************************************************************************************
*
First(relational operator) ={ ==,  != , > , < }
<relational operator>->
==  | 
!=   |  
>    |
<   
*
***************************************************************************************************************/
void relational_operator(void){
	switch(lookahead_token.code){
	case REL_OP_T:
		switch(lookahead_token.attribute.rel_op){
		case EQ:
			match(REL_OP_T,EQ);
			break;
		case NE:
			match(REL_OP_T,NE);
			break;
		case GT:/*>*/
			match(REL_OP_T,GT);
			break;
		case LT:/*<*/
			match(REL_OP_T,LT);
			break;
		default :
			break;
		}
		break;
	default:
		syn_printe();	/*new code*/
		break;
	}
}
/***************************************************************************************************************
*
First(input statement)={INPUT};
<input statement> ->
	INPUT (<variable list>);
*
***************************************************************************************************************/

void input_statement(void){
	
			match(KW_T,INPUT); match (LPR_T,NO_ATTR);
			variable_list(); 
			match(RPR_T,NO_ATTR);
			match(EOS_T,NO_ATTR);
			gen_incode("PLATY: INPUT statement parsed");
}

/***************************************************************************************************************
*
FIRST(variable list)={SVID_T,AVID_T}

<variable list> -> 	
				<variable identifier> <variable list_p>
*
***************************************************************************************************************/
void variable_list(void){
	switch(lookahead_token.code){	
	case AVID_T:
	case SVID_T:
		variable_identifier(); variable_list_p();
	break;
	default:
		syn_printe();
		break;
	}
		gen_incode("PLATY: Variable list parsed");
}
/***************************************************************************************************************
*
FIRST(variable list ' ) = {,| E }
<variable list ' >	,<variable identifier>  <variable list ' > 
			| e
*
***************************************************************************************************************/
void variable_list_p(void){

	if(lookahead_token.code == COM_T) {
		match(COM_T,NO_ATTR);
		variable_identifier(); variable_list_p();
	}
	/*
	switch(lookahead_token.code){
	case COM_T:
		match(COM_T,NO_ATTR);
		variable_identifier(); variable_list_p();
		break;
	default:
		break;
	}*/
}
/***************************************************************************************************************
*
First (variable_identfier)={SVID,AVID}
variable_identifier ->
	AVID | SVID 
*
***************************************************************************************************************/
void variable_identifier(void){
	switch(lookahead_token.code){
	case AVID_T:
		match(AVID_T,NO_ATTR);
		break;
	case SVID_T:
		match(SVID_T,NO_ATTR);
		break;
		
	default:
		/*syn_printe();*/
		break;
	}
}
/***************************************************************************************************************
*
First(output statement)={OUTPUT}
<output statement> ->	
	OUTPUT( <output list >);
*
***************************************************************************************************************/
void output_statement(void){
			match(KW_T,OUTPUT); match(LPR_T,NO_ATTR);
			output_list(); match(RPR_T,NO_ATTR);
			match(EOS_T,NO_ATTR);
			gen_incode("PLATY: OUTPUT statement parsed");
}
/***************************************************************************************************************
*
First (output list ) = {SVID_T,AVID_T,STR_T ,E }
<output list > -> 
		<variable list>
		|<string literal>
*
***************************************************************************************************************/
void output_list(void){
	switch(lookahead_token.code){
	case SVID_T:
	case AVID_T:
	variable_list();
	break;
	case STR_T:
		match(STR_T,NO_ATTR);
		gen_incode("PLATY: Output list (string literal) parsed");
		break;
	default:
		gen_incode("PLATY: Output list (empty) parsed");
	}
}
/***************************************************************************************************************
*
*FIRST(primary string expression )={ SVID,SL}
<primary string expression> ->
		<string variable identifier>
		| <string literal>
*
***************************************************************************************************************/
void primary_string_expression(void){
	switch(lookahead_token.code){
	case SVID_T:
		match(SVID_T,NO_ATTR);
		gen_incode("PLATY: Primary string expression parsed");
		break;
	case STR_T:
		match(STR_T,NO_ATTR);
		gen_incode("PLATY: Primary string expression parsed");
		break;
	default:
		syn_printe();
		break;
	}
}
/***************************************************************************************************************
*FIRST(primary  a_relational expression)={FPL,INL_Ts,AVID}

<primary a_relational expression> ->
			 <floating-point literal>
			| <integer literal>
			| <A variable identifier>
*
*
***************************************************************************************************************/
void primary_a_relational_expression(void){
	switch(lookahead_token.code){
	case FPL_T:
	case INL_T:
	case AVID_T:
		match(lookahead_token.code,lookahead_token.attribute.get_int);
		break;
	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Primary a_relational expression parsed");
}
/***************************************************************************************************************
*FIRST(primary s_relational expression>)={SL,SVID}
<primary s_relational expression> ->
			<string literal>|<primary string expression>
*s
*
***************************************************************************************************************/
void primary_s_relational_expression(void){
	
		primary_string_expression();
	gen_incode("PLATY: Primary s_relational expression parsed");
}
/***************************************************************************************************************
*FIRST(primary arithmetic expression)= {AVID,FPL,INR_T,(}			
<primary arithmetic expression> ->
				  <arithmetic variable identifier>
					| <floating-point literal>
					| <integer literal>
					| (<arithmetic expression>)	
*
*
***************************************************************************************************************/
void primary_arithmetic_expression(void){
	switch(lookahead_token.code){
	case AVID_T:
		match(AVID_T,NO_ATTR);
		break;
	case FPL_T:
		match(FPL_T,NO_ATTR);
		break;
	case INL_T:
		match(INL_T,NO_ATTR);
		break;
	case LPR_T:
		match(LPR_T,NO_ATTR);
		arithemetic_expression();
		match(RPR_T,NO_ATTR);
		break;
	default:
		/*syn_printe();*/
		break;
	}
	gen_incode("PLATY: Primary arithmetic expression parsed");
}
/***************************************************************************************************************
*
*
*
***************************************************************************************************************/

/********************** end of program *************************************************************************/

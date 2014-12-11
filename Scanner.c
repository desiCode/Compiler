/************************************************************************************************************************************
File name: scanner.c
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 2
Date: Oct 25, 2013 
Professor: Sv. Ranev
Purpose: The scanner reads the code written in the scanner character by character and then assigns a token 
		 according to the language specifications.It returns an error token when an unkownn character is encountered .Machine state is turned on 
		 when an alphabet or digit is recieved . State of the machine changes with respect to characters it receives from the buffer
		 and depend on the state of the machine accepting functions get called using the transition table.The transition table in table.h
		 also depicts the running of machine .
Function List:
scanner_init()
mlwpar_next_Token()
get_next_state()
char_class()
aa_func02(
aa_func03()
aa_func05()
aa_func08()
aa_func011()
aa_func013()
iskeyword()
 
************************************************************************************************************************************/
/******************HEADER FILES***********************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
#include <math.h>  /*to calculate octal*/
/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */
/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"
#include "stable.h"
/******************HEADER FILES***********************/
#define DEBUG  /* for conditional processing */
#undef  DEBUG
#define FAIL_F -2;
extern Buffer * str_LTBL;				/*String literal table */
extern STD sym_table;
int line;								/* current line number of the source code */
extern int scerrnum;				    /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */

static Buffer *lex_buf;						/*pointer to temporary lexeme buffer*/
static Buffer *err_buf;						/*pointer to temporary error buffer*/
/* scanner.c static(local) function  prototypes */ 
static int char_class(char c);						 /* character class function */
static int get_next_state(int, char, int *);		 /* state machine function */
static int iskeyword(char * kw_lexeme);			   	/*keywords lookup function */
static long atool(char * lexeme);					 /* converts octal string to decimal value */

int scanner_init(Buffer * sc_buf) {
  	if(b_isempty(sc_buf)) return EXIT_FAILURE;		/*1*/
	b_set_getc_offset(sc_buf,0);					/* in case the buffer has been read previously  */
	b_reset(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;							/*0*/
													/*   scerrnum = 0;  no need - global ANSI C */
}
/**********************************mlwpar_next_token***********************************************
Purpose: Reads a character from the scanner buffer created by the previous function.Processes each
		 character through certain conditions and return tokens according to the language specifications
		 If the symbol is not recognised an error token is generated.
Author: Lovejot Singh Dew
History/Versions: 25
Called functions: strncpy()
Parameters: sc_buf scanner buffer created and filed with code by the user !
Return value: Token with attribute if they exist
Algorithm: Read the char and check for validity returns a valid token for valid symbol else gerates 
			error token for unregonised symbols e.g. (~)
*********************************************************************************************/
Token mlwpar_next_token(Buffer * sc_buf)
{ 
   Token t;					/* token to return after recognition */
   unsigned char c;			/* input symbol */
   int state = ZERO;			/* initial state of the FSM */
   short lexstart;			 /*start offset of a lexeme in the input buffer */
   short lexend;			/*end   offset of a lexeme in the input buffer */
   int accept = NOAS;		/* type of state - initially not accepting */                                     
   
	int fnd=FALSE;				/*variable */
    int good=FALSE;				/*variable */
	char m[2]="\0";				/*temporary variable to hold chars for relations operators*/
	int loop;				/*loop variable */
	short  capacity=ZERO;		/*capacity for error buffer */
	while (1){ /* endless loop broken by token returns it will generate a warning */
          	
          /*GET THE NEXT SYMBOL FROM THE INPUT BUFFER*/
         c = b_getc(sc_buf);
		   
		 if(c=='\0'|| (unsigned)c == EOS){			/*check for end of file it can either be -1 or \0 */
			 t.code = SEOF_T;  /*set token code*/
			 return t;	
		 }
		 else if(c== '\n'){		/*check for next line also called line feed  */
			 ++line;			/*increment line number to count */
			 continue;
		 }
		 else if(c==' '||c=='\v'||c=='\t'||c=='\r'||c=='f')		/*check for spaces*/
			 continue;
		 else if(c==';'){		/*statement terminator*/
			 t.code=EOS_T;		/*token code*/
			 return t;
		 }
		 else if(c=='{'){		/*check for left brace */
			 t.code=LBR_T;		/*token code*/
			 return t;
		 }
		 else if(c=='}'){		/*check for right brace */
			 t.code=RBR_T;		/*token code */
			 return t;
		 }
		 else if(c=='('){		/*check for left parenthesis */
			 t.code=LPR_T;		/*token code*/
			 return t;
		 }
		 else if(c==')'){		/*check for right parenthesis  */
			 t.code=RPR_T;
			 return t;
		 }
		 else if(c==','){		/*check for comma  */
			 t.code=COM_T;
			 return t;
		 }
		 else if(c=='='){		/*check for assignment operator*/
			c=b_getc(sc_buf);	/*check to see if next char is also = which is known as relational operator*/
		  if(c=='='){			/*check for == operator*/
			 t.code=REL_OP_T;	
			 t.attribute.rel_op=EQ;		/*attribute for relational opertor */
			 return t;
		 }
		  b_retract(sc_buf);		/*retract buffer as getc offset move ahead with every char it returns */
		  t.code=ASS_OP_T;			/*token code*/
			 return t;
		 }
		 else if(c=='!'){			/*check for ! NOT operator also used to make comments*/
			 
			 c=b_getc(sc_buf);		/*take new char to check if its a comment of not-equal operator*/
			 if(c=='='){			/*check for notequal operator */
			 t.code=REL_OP_T;
			 t.attribute.rel_op=NE;
			 return t;
			 }
			 else if(c=='<'){			/*check for to see if it was comment*/
				 fnd=TRUE;				/*used like a boolean*/
				 while(c!='\n'){ 
					 c = b_getc(sc_buf);
					 if(c=='\n')++line;
					 if(c=='\0'||(unsigned)c == EOS)break;
				 }		/*discarding comment util a new line*/
			 }
			 else {				/*check for wrong comment syntax*/
					 t.code=ERR_T;		
					 lexstart=b_get_getc_offset(sc_buf)-2;
					 lexend=b_get_getc_offset(sc_buf);
					 b_set_getc_offset(sc_buf,lexstart);
					 loop=0;
					 for(lexstart;lexstart<lexend;lexstart++){
					 c=b_getc(sc_buf);
					 t.attribute.err_lex[loop]=c;
					  ++loop;
					 }
					 t.attribute.err_lex[loop]='\0';
					   while(c!='\n' || c=='\0'|| (unsigned)c == EOS){				/*filling error buffer  */
					 c = b_getc(sc_buf);			/*get character*/
					 if(c=='\n')++line;
					  }/*end of while*/
					  t.attribute.err_lex[ERR_LEN+1]='\0';
					 loop=0;
					 return t;
					 }/*end of else if*/
			
		 }
		 else if(c=='<'){			/*check for lessthan operator or it might be an stringconcatination symbol also*/
			 c=b_getc(sc_buf);		/*to check if the next symbol is >  */
			 if(c!='>'){
				 b_retract(sc_buf);		/*decrease getc_offset */
			 t.code=REL_OP_T;
			 t.attribute.rel_op=LT;
			 return t;
			 }
			 else {
				 t.code=SCC_OP_T;	/*token code for string concatination*/
				 return t;
			 }
		 }
		 else if(c=='>'){			/*check for greater than  */
			 t.code=REL_OP_T;
			 t.attribute.rel_op=GT;
			 return t;
		 }
		  else if(c=='+'){			/*check for arithmetic operator */
			 t.code=ART_OP_T;
			 t.attribute.arr_op=PLUS;
			 return t;
		 }
		  else if(c=='-'){		/*check for arithmetic operator */
			 t.code=ART_OP_T;
			 t.attribute.arr_op=MINUS;
			 return t;
		 }   
		  else if(c=='*'){			/*check for arithmetic */
			 t.code=ART_OP_T;
			 t.attribute.arr_op=MULT;
			 return t;
		 }
		   else if(c=='/'){			/*check for arithmetiv operator  */
			 t.code=ART_OP_T;
			 t.attribute.arr_op=DIV;
			 return t;
		 }
		 else if(c=='.'){		/*check for dot operator */
			  good=FALSE;		/*to see if its and .AND. .OR. */
			  m[0]=c;			/* array to hold characters */
			  c=b_getc(sc_buf);			/* get next character */
			  if(c=='A' || c=='O'){
				  if(c=='A')good=TRUE;
				  c=b_getc(sc_buf);		/* get next character*/
				  if((c=='N'&& good==TRUE) || (c=='R' && good == FALSE)){		/* check next character */
					 if(c=='R'){
		  			 c=b_getc(sc_buf);		/* get next character */
					 if(c=='.'){		 
						good=TRUE;			/* second . is found */
						t.code=LOG_OP_T;
						t.attribute.log_op=OR;
						return t;
 					 }
		 			 else{
									b_retract(sc_buf);
									b_retract(sc_buf);
									b_retract(sc_buf);
										t.code=ERR_T;	/* set the error token */
										strncpy(t.attribute.err_lex,m,ERR_LEN);
										return t;
					 }
				}
				else if(c=='N'){			/* check for N */
								c=b_getc(sc_buf);		/* get next character */
								if(c=='D'){				/* check for character */
									c=b_getc(sc_buf);	/* get next character */
									if(c=='.'){			/* check character */
										good=TRUE;		/* found the . operator */
									t.code=LOG_OP_T;	/* token code */
								t.attribute.log_op=AND;
								return t;
									}
											b_retract(sc_buf);
										b_retract(sc_buf);
										b_retract(sc_buf);
										b_retract(sc_buf);
										t.code=ERR_T;	/* set the error token */
										strncpy(t.attribute.err_lex,m,ERR_LEN);
										return t;
								}
								else {	/* or it can be a floating point */
										b_retract(sc_buf);
										b_retract(sc_buf);
										b_retract(sc_buf);
										t.code=ERR_T;	/* set the error token */
										strncpy(t.attribute.err_lex,m,ERR_LEN);
										return t;
								}
							}
						}
						else {	/* or it can be a floating point */
					b_retract(sc_buf);
					b_retract(sc_buf);
					t.code=ERR_T;	/* set the error token */
					strncpy(t.attribute.err_lex,m,ERR_LEN);
					return t;
						}
				}
				else {	/* or it can be a floating point */
					b_retract(sc_buf);
					t.code=ERR_T;	/* set the error token */
					strncpy(t.attribute.err_lex,m,ERR_LEN);
					return t;
				}
			 b_retract(sc_buf);
			}/*end of main else */
		  else if(c=='"'){				/* check for " quotes */
			  lexstart=b_get_getc_offset(sc_buf);		/* mark the beginning of the string*/
			c=b_getc(sc_buf);					/* get next character */
			if(c=='"'){							/* if its an empty string return */
			 t.code=STR_T;
			 t.attribute.str_offset=b_getsize(str_LTBL);
			b_addc(str_LTBL,'\0');				/* make it a c-type string */

			}
			else{					/* check for error or a valid string */
			 while(c!='"'){			/* check if we found second  ' " ' */
				 if(c=='\n')++line;
				 c=b_getc(sc_buf);		/* get next character */
				 
				 if(c=='\0' ||(unsigned)c == EOS){			/* if we hit end of file return error */
					 t.code=ERR_T;
					 /*attribute of error lexeme to be set*/
					 lexend=b_get_getc_offset(sc_buf)-1;		/*get the last char of the string */
					 capacity=lexend-lexstart;
					 b_set_getc_offset(sc_buf,lexstart-1);		/*-1 is to get the " and report them as error */
					 for(loop=ZERO;loop<=capacity;loop++){	
						c=b_getc(sc_buf);				/*get the next char */
					if((loop)<=(ERR_LEN-3))				/*check to see only 17 chars are added*/
						 t.attribute.err_lex[loop]=c;	/*setting error attribute*/
					 }/*end of if*/
					  t.attribute.err_lex[ERR_LEN-3]='.';	/*ending string */
					  t.attribute.err_lex[ERR_LEN-2]='.';	/*ending string */
					  t.attribute.err_lex[ERR_LEN-1]='.';	/*ending string */
					  t.attribute.err_lex[ERR_LEN]='\0';	/*making a C type string*/
				/*	 b_retract(sc_buf);			/*this killed my day and my marks */
					 return t;
					 }
			 }
			  lexend=b_get_getc_offset(sc_buf)-1;		/* mark the end of lexeme */
				 b_set_getc_offset(sc_buf,lexstart);	/* set the mark to begining of the string  */
				  t.code=STR_T;							/* set code*/
			 t.attribute.str_offset=b_getsize(str_LTBL);	/* return the offset where the string begins*/
			 for(loop=lexstart;loop<lexend;loop++){			/* loop to add chars to string buffer */
				 c=b_getc(sc_buf);							/* get next char*/
				
				 b_addc(str_LTBL,c);					/* add char to string buffer */
			 }/*end of for*/
			 c=b_getc(sc_buf);				/* get next character */
			 b_addc(str_LTBL,'\0');			/* adding \0 to make a ctype string */
			 lexstart=0;					/* reset */
			 lexend=ZERO;						/* reset */
			}/*end of else*/
			 return t;
		 }/*end of else if for "*/
		 else if(isalnum(c)){				/*if alphatber of digit */
			short capacity;					/*capacity for lexeme buffer*/
			 b_retract(sc_buf);				/*to get the offset of the first char*/
			b_setmark(sc_buf,b_get_getc_offset(sc_buf));		/*set mark when we hit first char of the lexeme */
			state=ZERO;			/*machine state o*/
			 do{
				 c=b_getc(sc_buf);			/*because we retracted the buffer before now get the same char again to runn the machine*/
				 state=get_next_state(state,c,&accept);		/*get next state of the machine depending on te char we sent */
				 if(accept != NOAS) break;			/*if the machine is in accepting mode break the loop*/
				 } while(accept==NOAS);			/*although while doing the same thing like if but its works with if there*/

			 lexstart = b_getmark(sc_buf);		/*get the lexeme offset is in the scanner buffer*/
			 if(accept == ASWR){				/*if the state is accepting with retract*/
			 b_retract(sc_buf);					/*retract the buffer*/
			 }/*end of if*/
			  lexend=b_get_getc_offset(sc_buf);			/*get last offset of the lexeme	*/
			  capacity=lexend-lexstart;					/*capacity of the buffer*/
			 lex_buf = b_create(capacity+1,1,'f');		/*creating a buffer*/
			 b_set_getc_offset(sc_buf,lexstart);		/*set buffer getc back to first char of lexeme*/
			 for(loop=lexstart;loop<lexend;++loop){		/* for loop to add  char into buffer */
				  c=b_getc(sc_buf);						/* get next character */
				/* if(c==' '||c=='\v'||c=='\t'||c=='\r'||c=='\f')	/*if any white space ignore it */
				/*	 b_addc(lex_buf,'\0');				/*i don't remember why i put this here */
				 b_addc(lex_buf,c);					/*adding c to lex buf*/
			 }/*end of for*/
			 b_addc(lex_buf,'\0');				/* adding */
			 t=aa_table[state](b_get_chmemloc(lex_buf,0));		/*calling appropraite function depending upon machine state*/
			 b_destroy(lex_buf);				/*destroy the buffer or clean the memory*/
			 return t;
		 } /*end of else if*/    
		 else{
			 t.code=ERR_T;
			 t.attribute.err_lex[0]=c;			/*wrong charracter */
			 t.attribute.err_lex[1]='\0';		/* making it a c-type string */
			 return t;
		 }
	}/*end while(1)*/
}
/*******************************************************************************************************************************************************
Purpose:  to get the char and return the state depend on the col of transition table
Author: S.Ranev
History/Versions: 0 Function was given
Called functions: char_class() assert() exit()
Parameters: state : current state of the machine
c : character sent from mlwpar() function
accept : accepting state of the machine
Return value: next : which is either NOAS,ASNR,ASWR 
Algorithm: get current state of machine put char in the column of transition table and change the state accordingly 
/********************************************************************************************************************************************************/

int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
printf("Input symbol: %c Row: %d Column: %d Next: %d \n",c,state,col,next);
#endif

assert(next != IS);

#ifdef DEBUG
	if(next == IS){
	  printf("Scanner Error: Illegal state:\n");
	  printf("Input symbol: %c Row: %d Column: %d\n",c,state,col);
	  exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}




/****************************************************************************************************************************************************
Purpose: THIS FUNCTION RETURNS THE COLUMN NUMBER IN THE TRANSITION
		TABLE st_table FOR THE INPUT CHARACTER c.
		SOME COLUMNS MAY REPRESENT A CHARACTER CLASS .
		FOR EXAMPLE IF COLUMN 1 REPRESENTS [A-Z]
		THE FUNCTION RETURNS 1 EVERY TIME c IS ONE
		OF THE LETTERS A,B,...,Z.
 
Author: S.Ranev
History/Versions: 3
Called functions: fgetc() b_addc()
Parameters: fi name of file
pBD pointer buffer descriptor
Return value: Count or failure is somthing went wrong 
Algorithm: Read in characters until end of file is found or the buffer is full.
/********************************************************************************************************************************************************/
int char_class (char c)
{
        int val=6; /*points to the other column in T_Table*/
		if( (isalpha(c))){			/*if c is alphabet column is 0 */
			val =0;
		}
		else if(isdigit(c)){		/*column 1 is for 0 and 2is for 1-7 and 3 is for 8-9 */
			if(c== '0'){
				val=1;
			}
			else if(c<='7')val =2;
			else if(c>='7' && c<= '9')val=3;
		}
		else if(c=='.')val=4;	/*decimal or dot operator goes to column 4*/
		else if(c=='#')val=5;	/* column 5 for SVID terminator #*/
		else val=6;				/*default column for other column*/
		return val;
}

/*****************************************************************************************************************************************************
Purpose: WHEN CALLED THE FUNCTION CHECKS IF THE LEXEME IS A KEYWORD.
		 IF YES, IT RETURNS A TOKEN WITH THE CORRESPONDING ATTRIBUTE
	     FOR THE KEYWORD.IF THE LEXEME IS NOT A KEYWORD,SET a AVID TOKEN.
		 IF THE lexeme IS LONGER than VID_LEN CHARACTERS,
		 ONLY FIRST VID_LEN CHARACTERS ARE STORED 
		 INTO THE VARIABLE ATTRIBUTE ARRAY vid_lex[](see token.h) .
	     ADD \0 AT THE END TO MAKE A C-type STRING.
Author: Lovejot Singh Dew
History/Versions: 2
Called functions: iskeyword() strncpy() strlen()
Parameters: lexeme[] : a string of lexeme or array of char contaning char
Return value: vid token  or keyword token 
Algorithm:  Takes a string checks it if its a keyword return a token accordingly or if it an vid then check for string
			length if the length is greater than VID_LEN truncate the string and copy it to vid_lex
/********************************************************************************************************************************************************/

Token aa_func02(char lexeme[]){
	
	Token t;
	int length;
	int index=iskeyword(lexeme);		/*calling iskeyword() to check if lexeme is a keyword*/

	if(index>=0){					/* if the index is greater than -1 we have found a keyword */
		t.code=KW_T;
		t.attribute.kwt_idx=index; 
		
	}/*end of if*/
	else{
		char lex[VID_LEN+1];
		int offset;
		length=strlen(lexeme);			/*length variable to hold length of the string*/
		
		if(length>VID_LEN){				/*a check for vid length*/
			strncpy(lex,lexeme,VID_LEN);			/*copying string 8 chars only discarding rest*/
			lex[VID_LEN]='\0';

		}	/*end of if*/
		else{							/*length smaller than VID_LEN*/
			strncpy(lex,lexeme,length);		/*copying string*/
			lex[length]='\0';				/*manualy adding terminator of string*/
		}	/*end of else*/
		t.code=AVID_T;						/*setting code for token*/

			offset=st_install(sym_table,lex,line);
		if(offset == FAIL_A){
		    printf("\nError: The Symbol Table is full - install failed.\n\n");
			st_store(sym_table);
			
			exit (EXIT_FAILURE);
		  }
		else if(offset == F_FAIL){
			++scerrnum;
			t.code=ERR_T;
			strcpy(t.attribute.err_lex,"RUN TIME ERROR! \0");
			return t;
	}
		  t.attribute.vid_offset=offset;

	}/*end of else*/
	
	/*
	printf(" |%s | \n"lexem);
	vertical bars shows exact output what was sent to function */
  return t;
}
/******************************************************************************************************************************************************
Purpose: WHEN CALLED THE FUNCTION SETs a SVID TOKEN .IF THE lexeme IS LONGER than VID_LEN 
		 characters,ONLY FIRST VID_LEN-1 CHARACTERS ARE STORED
		 INTO THE VARIABLE ATTRIBUTE ARRAY vid_lex[],
		 AND THEN THE # CHARACTER IS APPENDED TO THE NAME.
		 \0 is also added AT THE END TO MAKE A C-type STRING.
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: strnlen() strncpy()
Parameters: lexeme a string of lexeme
Return value: token of string literal 
Algorithm: takes a lexeme as a parameter checks if the length is with the boundaries if not then truncates the lexeme and sets token attribute and code and returns a token
/********************************************************************************************************************************************************/
Token aa_func03(char lexeme[]){
	Token t;
	int offset;						/*offset returning from the symbol table*/
	char nlex[VID_LEN+1];				/*array to hold strings*/
	int length;				
		length = strlen(lexeme);	/*getting length of lexeme*/
	if(length > VID_LEN){			/*checking length to be samller the vid_len*/
		strncpy(nlex,lexeme,VID_LEN);	/*truncating lexeme to vid_len*/
		nlex[VID_LEN-1]='#';				/*adding # for svid*/
		nlex[VID_LEN]='\0';
}/*	end of if*/
	else{
		strncpy(nlex,lexeme,length+1);	/*copying string */
		nlex[length+1]='\0';
	}	/*end of else*/
  t.code=SVID_T;		/*setting token code*/
  offset=st_install(sym_table,nlex,line);
  if(offset == FAIL_A){
	  printf("\nError: The Symbol Table is full - install failed.\n\n");
	  st_store(sym_table);
	 
	  exit (EXIT_FAILURE);			/*exit immediately*/
  }
  else if(offset == F_FAIL){
			++scerrnum;
			t.code=ERR_T;
			strcpy(t.attribute.err_lex,"RUN TIME ERROR! \0");
			return t;
	}
  t.attribute.vid_offset=offset;
  /*strncpy(t.attribute.vid_lex,nlex,VID_LEN+1);		/*setting svid lexeme */
  return t;		/*returning token*/
}
/*******************************************************************************************************************************************************
Purpose: take a lexeme and convert into a floating point number and then check for valid range and 
		 return valid token if its valid else return err token with lexeme as attribute 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: atof() strncpy()
Parameters: lexeme : a string of chars 
Return value: token for floating point number
Algorithm: convert string into floting point number using atof and the check to range if its a vlid range then 
			copy the string else truncate the length and then copy it also add \0 to make it c type 
********************************************************************************************************************************************************/
Token aa_func08(char lexeme[]){
	Token t;
	double value;		/*long variable to store floating point number and to check if its out of range*/
	value=atof(lexeme);
	if( value> FLT_MAX || (value <FLT_MIN && value > 0.0)) {	/*a check for overflow and underflow*/
		t.code=ERR_T;			/*setting error code */
		strncpy(t.attribute.err_lex,lexeme,ERR_LEN);	/*setting error attribute or sending error lexeme */
		t.attribute.err_lex[VID_LEN]='\0';
		}/*end of if*/
	else {
		t.code=FPL_T;		/*setting floating point code*/
		t.attribute.flt_value=(float)value;	/*setting floating point attribute */
	}/*end of else*/
  return t;
}
/*******************************************************************************************************************************************************
Purpose: take a lexeme and convert into a decimal number and then check for valid range and 
		 return valid token if its valid else return err token with lexeme as attribute 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: atoi() strncpy()
Parameters: lexeme : a string of chars 
Return value: token 
Algorithm: convert string into floting point number using atof and the check to range if its a vlid range then 
			copy the string else truncate the length and then copy it also add \0 to make it c type 
/********************************************************************************************************************************************************/
Token aa_func05(char lexeme[]){
	Token t;		
	int value = atol(lexeme); /*converting string into long int, for our scanner long int is 4 bytes */
	if(value <ZERO || value > PLT_MAX){
		t.code=ERR_T;
		strncpy(t.attribute.err_lex,lexeme,ERR_LEN); /*strncpy copyies the specified number of char to destination and if there is space for more char it add \0 automatically*/
		t.attribute.err_lex[ERR_LEN]='\0';			/*adding manually */
		return t;
	}
	else {
		t.code=INL_T;
		t.attribute.int_value=value;
		return t;
	}
	
}
/**********************************************************************************************************************************************************
Purpose: take a lexeme and convert into a octal using atool() then  decimal number ,again then check for valid range and 
		 return valid token if its valid else return err token with lexeme as attribute 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: atool() strncpy()
Parameters: lexeme : a string of chars 
Return value: valid token if any or error token if something went wrong 
Algorithm: convert string into decimal number using atool and then check to range if its a vslid range then 
			copy the string else truncate the length and then copy it also add \0 to make it c type 
/********************************************************************************************************************************************************/
Token aa_func11(char lexeme[]){
	Token t;
	short oct=(short)atool(lexeme);  /*atool returns a long decimal as the buffer ranges in short so its being cast to short there might be warning */
	if(oct >SHRT_MAX || oct < ZERO){
		t.code=ERR_T;
		strncpy(t.attribute.err_lex,lexeme,ERR_LEN+1);
		t.attribute.err_lex[ERR_LEN]='\0';
	}
	else {
		t.code=INL_T;
		t.attribute.int_value=oct;
	}
	return t;
}

/******************************************************************************************************************************************************
Purpose: to accept the lexeme with no retract 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: aa_table[]()
Parameters: lexeme a string of chars
Return value;  the lexeme will go to next function and return a token from there . 
Algorithm: pass the lexeme to next error function
/********************************************************************************************************************************************************/

Token aa_func12(char lexeme[]){
	
	aa_table[13](lexeme);
	/*there will be a warning about the return value */
	/*this will produce a warning*/
}
/*******************************************************************************************************************************************************
Purpose: to get the lexeme and report it as error truncate the string if its length is greater than ERE_LEN
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: strlen() strncpy()
Parameters: lexeme a string of chars
Return value: error token with chars 
Algorithm: Read in characters until end of file is found or the buffer is full.
/********************************************************************************************************************************************************/
Token aa_func13(char lexeme[]){
	Token t;
	
	char elex[ERR_LEN+1];				/*array to hold strings*/
	int length;
		length = strlen(lexeme);	/*getting length of lexeme*/
	if(length > (ERR_LEN-3)){			/*checking length to be samller the err_len*/
		strncpy(elex,lexeme,ERR_LEN);	/*truncating lexeme to vid_len*/
		elex[ERR_LEN]='\0';				/*adding . for error*/
	}/*	end of if*/
	else{
		strncpy(elex,lexeme,length);	/*copying string */
		elex[length]='\0';				/*adding . for error*/
	
	}	/*end of else*/
	t.code=ERR_T;
	strncpy(t.attribute.err_lex,elex,ERR_LEN+1);		/*setting svid lexeme */
	return t;
}
/*******************************************************************************************************************************************************
Purpose: to covert an octal string into an decimal number
Author: Lovejot Singh Dew
History/Versions: 2
Called functions: atoi() strlen() pow()
Parameters: 
Return value: Count or failure is somthing went wrong 
Algorithm:  call the string with atoi to convert it into integral form and then convert that octal number into
			into interger with custom logic.
/********************************************************************************************************************************************************/
long atool(char * lexeme){
	long int oct=0,rem=0,div=0;			/*variable needed for calculations*/
	int vloop=0;					/*loop variable*/
	int noct=atoi(lexeme);			/*octal variable to hold octal value*/
	int length =strlen(lexeme);		/*we get the length of octal number and a condition for our loop*/
	
	for(vloop=0;vloop<=length;++vloop){
		rem=noct%10;				/*% returns remainder*/
		div=noct/10;				/*truncates the last digit fromt he number*/
		oct=oct +rem*(int)pow(8,vloop);	/*calculation for the number this will produce a warning*/
		noct=div;					/*assign the truncated number to noct */
		if(div <= 0 )break;			/*there is no more digit to truncate*/
	}
	return oct;
}

/*******************************************************************************************************************************************************
Purpose: to check  if the string is a keyword or not
Author:					Lovejot Singh Dew
History/ Versions:		 2
Called functions:		strcmp()
Parameters:				kW-lexeme a set of chars
Return value:			index of the keyword if found or  R_FAIL_1 which is -1 on not found condition 
Algorithm:			    take the string and compare it with the array kw_table which contains keywords
/********************************************************************************************************************************************************/
int iskeyword(char * kw_lexeme){
	int loopV = 0; /*loop variable*/
	int index=-1;	/*index intialized to -1 */
	
	for(loopV=0;loopV<KWT_SIZE;++loopV){		
		if(strcmp(kw_table[loopV],kw_lexeme)==ZERO){	/*string compare in a loop */
			index=loopV;								/*if found give the value of loopv to index*/
			break;
			}
	}
	if(index>=0)return index;				/*i can remove this if too but let it be there */
	else						
		return R_FAIL_1;
}
/************************************************************** END OF PROGRAM **************************************************************************/

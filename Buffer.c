/*header file/s*/
#include"buffer.h"
/*********************************************************************************************
File name: buffer.c
Compiler: Microsoft Visual Studio 2010 and GCC 
Author: Lovejot Singh Dew
Course: CST 8152 ñ Compilers Lab Section: 402 
Assignment: 1 
Date: Oct 2, 2013 
Professor: Sv. Ranev
Purpose: This file contains the function definitions for the Buffer
Function list: 
b_create() 
b_addc()
b_reset()
b_destroy()
b_isfull()
b_getsize()
b_getcapacity()
b_setmark()
b_getmark()
b_getmode()
b_load()
b_isempty()
b_eob()
b_getc()
b_print()
b_pack()
b_get_r_flag()
b_retract()
b_get_getc_offset()
b_set_getc_offset()
b_get_chmemloc()
*********************************************************************************************/

/*********************************************************************************************
Purpose: To initialize a BufferDescriptor on the Heap 
Author: Lovejot Singh Dew
History/Versions: 7
Called functions:calloc() malloc()  free()
Parameters: init_capacity 0 to Short Max assuming init_capacity to be number of elements 
inc_factor depending on mode
o_mode 'f', 'a', 'm' 
Return value: Pointer to a BufferDescriptor  NULL if failure
Algorithm: Check for valid buffer pointer, before allocating memory.
*********************************************************************************************/
Buffer * b_create(short init_capacity,char inc_factor,char o_mode)
{
	Buffer * nBuffer=NULL;
	if(init_capacity<=0 ){return NULL;}

 nBuffer=( Buffer  *)calloc( ONE,sizeof(Buffer));
if(nBuffer==NULL){return NULL;}
	
	nBuffer->ca_head=(char *)malloc(sizeof(char)*(init_capacity));/*allocation memory to buffer with the space required for number of chars*/
	if(nBuffer->ca_head==NULL){
		free(nBuffer);
		return NULL;}

	if(o_mode == 'f' || inc_factor==ZERO){/*no need to increase the capacity just add the characters and finish*/

		nBuffer->mode=ZERO;/*set mode to zero*/
		nBuffer->inc_factor=ZERO;/*set incfator to zero didn't use sizeof() as 0 elements will have 0 bytes of memory*/
		nBuffer->capacity=init_capacity;/*set capacity to fixed capacity didn't use sizeof(0 elements will have 0 bytes of memory) */
		return nBuffer;/*returning buffer pointer */
	}/*end of if*/
	
	else if(o_mode =='a'){/*additive mode*/
			nBuffer->mode= SET_R_FLAG;/*set mode to one*/
			nBuffer->inc_factor=inc_factor;/*set inc_factor to bytes of memory needed*/	
			nBuffer->capacity=init_capacity;/*set capacity to bytes of memory needed*/
			return nBuffer;/*return buffer pointer */
	}
	else if(o_mode =='m'&& inc_factor >ZERO || inc_factor <=100 ){/* for multiply mode*/
			nBuffer->mode =  R_FAIL_1;/*set mode ot -1*/
			nBuffer->inc_factor=inc_factor;/*set inc_factor*/
			nBuffer->capacity=init_capacity;/*set capacity*/
			return nBuffer;/*returning buffer pointer*/
	}
	else {/*if no mode specified lets go home */
		free(nBuffer->ca_head);
		free(nBuffer);
		return nBuffer;
	}
}



/*********************************************************************************************
Purpose: To add a character to the buffer 
Author: Lovejot Singh Dew
History/Versions: 4
Called functions: realloc() sizeof() b_isfull()
Parameters: init_capacity 0 to Short Max  
inc_factor assuming 0 to 255 
o_mode 'f', 'a', 'm' 
Return value: Pointer to a BufferDescriptor 
Algorithm: Check for valid buffer pointer, before allocating memory.Then proceed with adding characters
*********************************************************************************************/
Buffer* b_addc(Buffer* const pBD,char symbol){

	char * tBuffer;
	short capacity=0;/*temporary short to hold new memory calculations*/
	short avail,nInc;	/*temporary short to calculate available space and new Icrement */
	
	
	if(pBD->addc_offset==SHRT_MAX)return NULL;
	pBD->r_flag=0;
	if(b_isfull(pBD)!=ONE){ /*if buffer is not full add char to it*/
		
			pBD->ca_head[pBD->addc_offset]=symbol;	/*add symbol to buffer*/
			++pBD->addc_offset;						/*increase offset*/
			return pBD;								/*return buffer pointer*/
	}
	else {
 		if(pBD->mode ==ZERO){/*mode zero we do nothing*/
			return NULL;
		}/*end of if */
		else if(pBD->mode == M_A){	/*addition mode*/	
			capacity=(unsigned char )pBD->inc_factor+pBD->capacity;/*taking inc_factor as number of elements and calculatin space using sizeof . capacity is already in bytes*/
			if(capacity <0) return NULL;	
		}/*end of else if */
		else if(pBD->mode == M_M){/*multiplication mode*/
			avail = SHRT_MAX - pBD->capacity;/*available capacity*/
			 nInc = (short)((float)(avail * pBD->inc_factor)/100.0f);/*calculating space for multiplication and casting the whole number as float*/
			capacity = pBD->capacity +nInc;/*total capacity of new buffer*/

		}/*end of else if */

		if(capacity==pBD->capacity){capacity = SHRT_MAX;}
		tBuffer=(char *) realloc(pBD->ca_head,capacity);/*increasing size of buffer */
		if(tBuffer ==NULL){/*something went wrong */
			return NULL;/*we get nothing*/
		}/*end of if */
		if(tBuffer!=pBD->ca_head){
		
			pBD->r_flag=SET_R_FLAG;/*set r_flag because of realloc new memory address has been assigned to */
			pBD->ca_head=tBuffer;/*assigning temporary buffer to ca_head*/
		}
			pBD->capacity=capacity;/*assignment new capacity to data member of buffer structure*/
			pBD->ca_head[pBD->addc_offset]=symbol;/*buffer size increased so add the charactor to it*/
			++pBD->addc_offset;/*increase offset as the charater has been added*/
		/*end of else*/
		return pBD;/*return buffer pointer*/
	}/*end of else */

}/*end of function */



/*******************************************************************************************************************************************
Purpose: To read characters from the file to the buffer 
Author: Lovejot Singh Dew
History/Versions: 3
Called functions: fgetc() b_addc()
Parameters: fi name of file
pBD pointer buffer descriptor
Return value: Count or failure is somthing went wrong 
Algorithm: Read in characters until end of file is found or the buffer is full.
/********************************************************************************************************************************************************/

int b_load(FILE* const fi,Buffer* const pBD){
	int count=ZERO;/*temporary count of data type int to count number of character added to the buffer\ */
	char tchar;/*temporary varaible of type char to hold charater read from file */

	while ((tchar =(unsigned char) fgetc(fi)) != EOF) {/*until you find the needle called endoffile keeping searching the hay */
		if(b_addc(pBD,tchar)==NULL){/*add characters to the buffer untils its full or something went wrong*/
		return LOAD_FAIL;/*if something is wrong tell me now*/
	 }
		count++;/*a char was added*/
	}
	return count;/*total number of chars added*/
}/*end of function*/

/*******************************************************************************************************************************************
Purpose: To reset the data members of buffer structure 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: none
Parameters: pBD pointer buffer descriptor
Return value: 1(r_FAIL_1) is something is wrong or 0(ZERO)
Algorithm:check if the buffer pointer is valid and reset all required data members
/********************************************************************************************************************************************************/
int b_reset(Buffer* const pBD){
	if(pBD==NULL){/*buffer pointer is not valid shout out*/
		return R_FAIL_1;
	}else{
	pBD->addc_offset=ZERO;	/*reset data member to zero*/
	pBD->getc_offset=ZERO; /*reset data member to zero*/
	pBD->mark_offset=ZERO;	/*reset data member to zero*/
	pBD->r_flag=0;/*set r_flag because of realloc new memory address has been assigned to */
	pBD->eob=ZERO;/*reset data member to zero*/
	return ZERO;		/*reset data member to zero*/

	}
}
/*******************************************************************************************************************************************
Purpose: To reset the data members of buffer structure 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: none
Parameters: pBD pointer buffer descriptor assuming it to be a valid pointer
Return value:
Algorithm:check if the buffer pointer is valid and reset all required data members
/********************************************************************************************************************************************************/
void  b_destroy(Buffer* const pBD){
	if(pBD->ca_head!=NULL){
		free(pBD->ca_head);/*release memory allocated to buffer*/
		free(pBD);/*free buffer */
	}
}
/*******************************************************************************************************************************************
Purpose: To check if the buffer is full or not
Author: Lovejot Singh Dew
History/Versions: 2
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: ZERO if something is wrong , ONE is buffer is full or else -1 is anything is wrong  
Algorithm:assuming buffer pointer to be valid ,check if the addc_offset is not at last offset , or if addc_offset went off 
/********************************************************************************************************************************************************/
int b_isfull(Buffer * const pBD){
	if(pBD->ca_head==NULL) return FAIL_1;
	else if(pBD->addc_offset < pBD->capacity){/*addc_offset is in the limit*/
		return ZERO ;/*yes this things works*/
	}
	else if(pBD->addc_offset == pBD->capacity){/*buffer is full*/
		return  SET_R_FLAG;	/*buffer is full*/
	}
	else{ return  R_FAIL_1;/*something went wrong*/
	}
}
/*******************************************************************************************************************************************
Purpose: to get current size of the filled buffer
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: ZERO if something is wrong , ONE is buffer is full or else -1 is anything is wrong or   
Algorithm:assuming buffer pointer to be valid ,check if the addc_offset is not at last offset , or if addc_offset went off  
/********************************************************************************************************************************************************/
short b_getsize(Buffer* const pBD){

	if(pBD->ca_head!=NULL){
		return  pBD->addc_offset;	/*buffer is full*/
	}
	else{ return  R_FAIL_1;/*something went wrong*/
	}
	
}
/*******************************************************************************************************************************************
Purpose: to get size of the buffer
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: NULL is its not a valid pointer or capacity of buffer   
Algorithm: assuming buffer pointer to be valid ,check if the addc_offset is not at last offset , or if addc_offset went off  
/********************************************************************************************************************************************************/
short b_getcapacity(Buffer * const pBD){

	
	if(pBD->ca_head!=NULL){
	return pBD->capacity; /*total capacity of the buffer*/
	}
	else return FAIL_1;
}
/*******************************************************************************************************************************************
Purpose: to set mark 
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
mark : mark to be set
Return value: NULL is its not a valid pointer or capacity of buffer   
Algorithm: assuming buffer pointer to be valid ,check if mark is not greater than addc_offset  
/********************************************************************************************************************************************************/
int b_setmark(Buffer* const pBD,short mark){
	if(mark > pBD->capacity || pBD->ca_head==NULL){
		return  R_FAIL_1;
	}
	else {
		pBD->mark_offset=mark;
		return pBD->mark_offset;
	}
}
/*******************************************************************************************************************************************
Purpose: to get mark
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: NULL is its not a valid pointer or capacity of buffer   
Algorithm: assuming buffer pointer to be valid ,return mark_offset 
/********************************************************************************************************************************************************/
short b_getmark(Buffer* const pBD){
	
	if(pBD->ca_head!=NULL){
		return pBD->mark_offset;/*we get mark_offset*/
	}
	else return FAIL_1;
}
/*******************************************************************************************************************************************
Purpose: to get mode
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: NULL is its not a valid pointer or capacity of buffer   
Algorithm: assuming buffer pointer to be valid ,return mode 
/*********************************************d***********************************************************************************************************/
int b_getmode(Buffer* const pBD){
	
	if(pBD->ca_head!=NULL){
	return pBD->mode;/*gives us mode of increase*/
	}
	else {return (FAIL_1);}
}
/*******************************************************************************************************************************************
Purpose: to check if the buffer is empty or not
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: zero which is fail or One if its empty    
Algorithm: assuming buffer pointer to be valid ,check is addc_offset it still at beginning of the function
/********************************************************************************************************************************************************/
int b_isempty(Buffer* const pBD){
	if(pBD->addc_offset==ZERO)	/*check if addc_offset is still at the begining of the buffer*/
			return  SET_R_FLAG; /*return one is its empty*/
	else return ZERO;/*its not empty*/
}
/*******************************************************************************************************************************************
Purpose: to check if the buffer has reached end of buffer
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: eob either one or zero   
Algorithm: assuming buffer pointer to be valid ,return endofbuffer data member
/********************************************************************************************************************************************************/
int b_eob(Buffer* const pBD){
	
	if(pBD->ca_head!=NULL){
	return pBD->eob;
	}
	else return FAIL_1;
}
/*******************************************************************************************************************************************
Purpose: to get the character in the buffer at getc_offset
Author: Lovejot Singh Dew
History/Versions: 3
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: eob either one or zero   
Algorithm: assuming buffer pointer to be valid ,return endofbuffer data member
/********************************************************************************************************************************************************/
char b_getc(Buffer* const pBD){
	short temp;
	if(pBD->ca_head==NULL){ /* check for valid buffer pointer*/
		return LOAD_FAIL;	/*failure at getting  char from the buffer*/
	}/*end of if*/
	else if(pBD->addc_offset==pBD->getc_offset){/*check is all the characters have been retrived */
		pBD->eob= ONE;	/*all the chars are retreived*/
		return  R_FAIL_1;	/*nothing more can be read so stop */
	}/*end of else if*/
	else {
		pBD->eob=ZERO;/*not end of buffer*/
	temp = (pBD->getc_offset);/*store char offset in temporary short temp*/
	pBD->getc_offset++;	/*increase gect offset as we read that char*/
	return pBD->ca_head[temp];	/*retreive the char from the offset*/
	}/*end of else*/
}/*end of funtion*/
/*******************************************************************************************************************************************
Purpose: to print the chars in the buffer 
Author: Lovejot Singh Dew
History/Versions: 3
Called functions: b_set_getc_offset() b_getc()printf()
Parameters: pBD pointer buffer descriptor
Return value: eob either one or zero   
Algorithm: assuming buffer pointer to be valid ,return count of chars printed
/********************************************************************************************************************************************************/
int b_print(Buffer* const pBD){
	int temp=ZERO;	/*temporary short to count number of chars*/
	char tchar;		/*temporary char to store the char read from the buffer*/
	if(pBD->addc_offset==ZERO){		/*nothing to print*/
		printf("The buffer is empty.\n");
		return  R_FAIL_1;
	}/*end of if*/
	else {
		b_set_getc_offset(pBD,ZERO); /*calling function to set the value to getc to zero*/
		while(ONE){/*infinte loop this will generate a warning*/
			tchar=b_getc(pBD);	/*read char from buffer and assign it to tchar*/
			if(pBD->eob==ONE || tchar==R_FAIL_2){/*if end of buffer is reached or somthing went wron in getc funtion shout*/
				break;	/*quit while loop*/
			}/*end of if*/
		printf("%c",tchar);	/*if a valid char was read print it*/
		++temp;		/*count of chars increasing as it gets printed*/
		}/*end of else*/
		printf("\n");	
	}
		
		return temp;	/*return number of char printed*/
	
}/*end of function*/
/*******************************************************************************************************************************************
Purpose: to increase the size of buffer to hold one more char 
Author: Lovejot Singh Dew
History/Versions: 5
Called functions: reaclloc()
Parameters: pBD pointer buffer descriptor
Return value: NULL on failure or pointer buffer descriptor
Algorithm: assuming buffer pointer to be valid , increase the size of buffer to one more char
/********************************************************************************************************************************************************/
Buffer *b_pack(Buffer* const pBD){

	short temp;	/*temp variale of type short*/
	char * tBuffer;	/*tBuffer pointer of type char*/
	pBD->r_flag=0;


	if(pBD->addc_offset==SHRT_MAX)return NULL;
	
	
	temp=(pBD->addc_offset +ONE)*sizeof(char);	/*to pack buffer with one more char*/

	tBuffer =(char *)realloc(pBD->ca_head,temp); /*temoparary buffer is allocated increased space and assigned the address of new memory*/
	
	if(tBuffer== NULL)return NULL;

	if(tBuffer!= pBD->ca_head){	/*check to see if realloc worked fine*/
		pBD->ca_head=tBuffer;	/*assign tBuffer to ca_head of the buffer*/
		pBD->r_flag=1;/*set r_flag because of realloc new memory address has been assigned to */
		b_addc(pBD,'\0');
	}
		pBD->capacity=temp;		/*temp is the new capacity of the buffer*/
	
		return pBD;			/*return pointer descriptor*/
	

}
/*******************************************************************************************************************************************
Purpose: to get r_flag
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: r_flag
Algorithm: assuming buffer pointer to be valid , increase the size of buffer to one more char
/********************************************************************************************************************************************************/
char b_get_r_flag(Buffer* const pBD){

	if(pBD->ca_head!=NULL)
			return pBD->r_flag;
	
	else return FAIL_1;
}

/*******************************************************************************************************************************************
Purpose: to decrease getc_offset by 1
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: -1 on failure or getc_offset 
Algorithm: assuming buffer pointer to be valid , decrease the getc_offset by one
/********************************************************************************************************************************************************/
int b_retract(Buffer* const pBD){
	if(pBD->getc_offset==(ZERO)){
		return  R_FAIL_1;
	}
	else{
	--pBD->getc_offset;
	return pBD->getc_offset;
	}
}

/*******************************************************************************************************************************************
Purpose:  get getc offset
Author: Lovejot Singh Dew
History/Versions: 1
Called functions: NONE
Parameters: pBD pointer buffer descriptor
Return value: -1 on failure or getc_offset 
Algorithm: assuming buffer pointer to be valid ,  returns getc_offset 
/********************************************************************************************************************************************************/
short b_get_getc_offset(Buffer* const pBD){
	if(pBD->ca_head!=NULL)
		return pBD->getc_offset;
	else return FAIL_1;

}
/*******************************************************************************************************************************************
Purpose: to set getc offset
Author: Lovejot Singh Dew
History/Versions: 5
Called functions: reaclloc()
Parameters: pBD pointer buffer descriptor
offset new offset to set 
Return value: 1 on failure or Zero on success
Algorithm: assuming buffer pointer to be valid , increase the size of buffer to one more char
/********************************************************************************************************************************************************/
int b_set_getc_offset(Buffer* const pBD,short offset){

	if(offset<pBD->capacity || pBD->ca_head!=NULL){
		pBD->getc_offset=offset;
		return ZERO;
	}
	else return ONE;
}
/*******************************************************************************************************************************************
Purpose: to increase the size of buffer to hold one more char 
Author: Lovejot Singh Dew
History/Versions: 5
Called functions: reaclloc()
Parameters: pBD pointer buffer descriptor
Return value: NULL on failure or pointer buffer descriptor
Algorithm: assuming buffer pointer to be valid , increase the size of buffer to one more char
/********************************************************************************************************************************************************/
char * b_get_chmemloc(Buffer* const pBD,short offset){
	if(offset > pBD->capacity){ 
	return NULL;
	}
	else
	{ 
		 return pBD->ca_head+offset;
	}
}

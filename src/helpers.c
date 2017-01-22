/*
 * helpers.c
 *
 *  Created on: 2011/11/19
 *      Author: cj-chang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "helper.h"

//#include <unistd.h>
//#include <sys/types.h>
//#include <dirent.h>
//#include <string.h>

#include "helper.h"
#define lnx(cmdBuf) linux_system_cmd_dump(__FILE__, __LINE__, cmdBuf)
#define spr sprintf

void put_line_to_lfile_head(char *file, char *line)
{
	char cmd[128];
	if(fopen("tmp", "r") == NULL){
		spr(cmd, "echo %s > tmp", line) ;
		lnx(cmd) ;
		spr(cmd, "cat %s >>tmp", file);
		lnx(cmd) ;
		spr(cmd,"cp tmp %s", file) ;
		lnx(cmd) ;
		spr(cmd, "rm tmp") ;
		lnx(cmd);
	}
	else{
		SD2("put_line_to_lfile_head can use tmp", line) ;
		fatal_end(__LINE__, __FILE__);
	}
}

void end(int line_number)
{
	//printf("      >:<\n");
	printf("===============\n");
	printf("end at line %d\n", line_number );
	exit(0);
}



void end2(int line_number, char *filename)
{
	printf("\nException Catched, Exit @%s:%d\n", filename,line_number );
	exit(0);
}

void alloc_cstring(char *pt, int num)
{
	pt= malloc((num+1) * sizeof( char));
	pt= num ;
}

//Functions pointers in C

/*
int addInt(int n, int m) {
    return n+m;
}
int (*functionPtr)(int,int);
functionPtr = &addInt;
*/

int isNode(char *p) {return (*p=='/' && p[strlen(p)-1] == '/');}    //[/a/b/c/]
int isHat(char *p) {return (*p=='/' && p[strlen(p)-1] != '/');}     //[/a/b/c]
int isBridge(char *p){ return (*p!='/' && p[strlen(p)-1] != '/');}  //[a/b/c ]
int isPath(char *p) {return (p[0]=='/' && p[strlen(p)-1] != '/') ; }	 //hat as path
int isLeaf(char *p) { return (*p!='/' && p[strlen(p)-1] != '/');}   //leaf is as bridge

void chkGoodSeg(int funPtr(), char *seg, char *note ){
	printf("[%c][%c]\n", *seg, seg[strlen(seg-1)]);
	char msg[256]= {'\0'} ;
	strcpy(msg, seg);
	strcat(msg, note);
	if(!funPtr(seg))
	{
		SD2( msg, seg) ;
		exit(0);
	}
}

void putHat2Node(char *pNode, char* pHat)
{
	//char aBuf[]="/1/2/3/";
	//pNode = aBuf ;
	chkGoodSeg( &isHat, pHat, " is not a Hat");
	SD2("pHat", pHat) ;
	chkGoodSeg( &isNode, pNode, " is not a Node");
	sD(__LINE__, "pNode", pNode) ;
	exit(0);
	if(!isNode(pNode))
	{
		SD2( "Not a Node", pNode) ;
		exit(0);
	}
	if(!isHat(pHat))
		{
			SD2( "Not a hat", pHat) ;
			exit(0);
		}
	if(1){
		char aBuf[256];
		strcpy(aBuf,pHat );
	    strcat(aBuf, pNode);
	    strcpy(pNode, aBuf);
	}
}

void bridgeNode2Path(char *pNode, char *pBridge, char *pPath)
{
	char aBuf[256];
	strcpy(aBuf, pNode);
	strcat(aBuf, pBridge);
	strcat(aBuf, pPath);
	strcpy(pPath, aBuf);
}
void putHat2Path(char *pPath, char *pHat)
{
	if(!isPath(pPath)) {SD2("Not a path", pPath); exit(0);}
	if(!isHat(pHat))    {SD2("Not a hat",  pHat) ; exit(0);}

	{
		char aBuf[256] ;
		strcpy(aBuf, pHat);
		strcat(aBuf,pPath) ;
		strcpy(pPath, aBuf);
	}
}

void AppendLeaf2Node(char *pNode, char* pLeaf)
{
	if(!isNode(pNode)) {SD2("Not a Node", pNode); exit(0);}
	//if(!isLeaf(pLeaf)) {SD2("Not a Leaf",  pLeaf); exit(0);}
	strcat(pNode, pLeaf);
}

void sD(int lineNumber, char *pToken, char *p)
{
	if((strlen(pToken) +strlen(p))< 60)    //why 60
	{
		printf("\n  [%d] id:%s -> %s\n", lineNumber, pToken, p);
	}
	else
	{
		printf("\n  [%d] %s ->\n   %s\n", lineNumber, pToken, p);
	}
}

void POP_FAIL(char *varID)
{
	char buf[256];
	sprintf(buf, "\"%s\"", varID);
	strcat(buf, varID);
	SD2(buf, varID);
}

//sD2 cannot be nullified!!
void sD2(int lineNumber, char* fileLeaf, const char *pToken, const char *p)
{

	//if((strlen(pToken) +strlen(p))< 80)    //why 80
	//{
		//printf("\n id:[%s], line:[%d], src:<%s> -> %s\n",  pToken, lineNumber,fileLeaf, p);
	//}
	//else
	//{
		char token[256];
		sprintf(token, "ID (code):%s,",pToken);
		//printf("\n ID:%-20s, LINE:[%04d], SRC:[%s]\n   ->%s\n", pToken, lineNumber,fileLeaf,  p);
		//printf("\n %-20s LINE:[%04d], SRC:[%s]\n     ->%s\n", t, lineNumber,fileLeaf,  p);
		//printf("\n %-20s \n     @%s :%04d, \n        TAG--> %s\n", token, fileLeaf,lineNumber,  p);

		printf("\n      @%s >> %04d <<, \n",fileLeaf,lineNumber);
		printf("       %-20s \n", token);
		printf("       Value --> %s\n", p);
	//}
}
/*
void sfD(int lineNumber, char* filePath,  char *pToken, char *p)
{
	printf("\n  (%s)[%d] %s ->\n   %s\n",filePath, lineNumber,   pToken, p);
}
*/
void sh_out_variable( char *cmd,  char *var)
{
	FILE *fp;
	char leaf[]="a_tmp_file";
	char cmdBuf[256];
	sprintf(cmdBuf, "%s > %s", cmd, leaf ) ;
	//SD2("cmdBuf", cmdBuf) ;
	system(cmdBuf);
	linux_system_cmd_dump(__FILE__, __LINE__, cmdBuf);

	if( NULL != (fp = fopen(leaf, "r")))
	{
		fscanf(fp, "%s", var);
		SD2("var", var);
		fclose(fp);
	}
	else
	{
		END2 ;
	}
	char rm_leaf_cmd[256] ;
	sprintf(rm_leaf_cmd, "rm %s", leaf);
	linux_system_cmd_dump(__FILE__, __LINE__, rm_leaf_cmd);
}


int checkIfDirExisted(const char * const theDir)
{
	DIR* dir = opendir(theDir);
	if (dir)
	{
		/* Directory exists. */
		closedir(dir);
		return(DIR_ALREADY_EXIST);
	}
	else
	{
		return (DIR_NOT_EXIST);
	}
}

char* read_id(char *student_id, int len,  FILE *f_pt)
{
	static int members_count = 0 ;
	int aI=0 ;
	char buf[128];

	do{
		fgets(buf, len, f_pt);
		if( buf[0]== 10 || buf[0]==EOF|| buf[0]=='&')    //for compatibility of unix and dos
		{
			printf("\n\n(StudentS-list Ended by line header [%c] and the Checked ASCII Code is:[0X%x])\n(Members graded: %d)\n\n ", buf[0], buf[0], members_count);	//echo the terminated criteria
			return(NULL);
		}
	}while (buf[0]=='*');

	{
		while(buf[aI] != '/')
		{
			student_id[aI] = buf[aI];
			aI++ ;
		}
		student_id[aI]='\0';
		printf("\n\n***********************************\nThe Next Student_ID: %s\n***********************************\n",student_id);
	}
	members_count++;
	return(student_id);
}

//append a delimiter to file
void cat_left_file_append_2_right_file(char* dlt_file_name, char *file_name)
{
char aCmd[512];
		sprintf(aCmd,	"cat %s >> %s",dlt_file_name, file_name ) ;
		SD2( "sh executed inside x_cat_dlt_by_file()", aCmd) ;
		linux_system_cmd_dump(__FILE__, __LINE__, aCmd);
}

void append_left_string_2file(char* pStr, char *file_name)
{

int i ;
char new_str[1024];
int new_i=0 ;
int flag =0 ;
		strcpy(new_str, pStr);

		for(i=0; i<= strlen(pStr); i++){
			new_i++ ; ;
			if(pStr[i] == '\\'){
				flag=1 ;
				//new_str[new_i++] = '\"';
				//new_str[new_i++]= '\\';
				new_str[new_i]= '\\';
				//new_str[new_i]='\"';
			}
			else new_str[new_i-1]=pStr[i];
		}
		new_str[new_i]='\0';

		for(i=0; i<= strlen(new_str); i++){
			if(new_str[i]=='\n'){
				new_str[i]='#';
			}
		}


			FILE *fp_2=fopen(file_name,"a");
			fprintf(fp_2,"%s\n", new_str);
			fclose(fp_2);

}


void append_string_2fileS(int n, ...)
{
	va_list ap;
	int i;
	char *data_str ;
	char *current_path ;

	va_start(ap, n) ;
	data_str = va_arg(ap, char *) ;
	printf("data_str is:[%s]\n", data_str) ;
	for(i=1; i<n; i++) {
		current_path = va_arg(ap, char *) ;
		printf("current_path is:[%s]\n", current_path) ;
		append_left_string_2file(data_str, current_path ) ;
	}
}

///* Exist while not a good unix command

void linux_system_cmd_dump(char *file, int line, char *cmd)
{
	SD2("cmd",cmd);
	unsigned ret;
	ret = system(cmd) ;

	if(ret !=0) {
		char buf[128];
		sprintf(buf, "exit: A Failed Unix Command, Return Flag[%x] @%s:%d", ret,  file, line ) ;
		printf("\n%s\n      #%s\n", buf, cmd) ;
		fatal_end(__LINE__, __FILE__) ;
		//exit(0) ;
	}
}



/*
void system_call(char *s)
{
	unsigned ret;
	ret = system(s) ;
	if(ret !=0) {
		printf("%d\n", ret) ;
		SD2("error \"/bin/sh -c command\"" ,s)  ;
		END2 ;
	}
}
*/

int max_int(int n, ...)
{
	va_list ap;
	int i, current, largest;

	va_start(ap, n) ;
	largest = va_arg(ap, int) ;
	for(i=1; i<n; i++) {
		current = va_arg(ap, int) ;
		if(current > largest)
			largest = current ;
	}
	return largest ;
}



void dlt_2_file(const char *dst_file_path, char *fmt_folder, char*fmt_file)
{
    char dlt[128];
    sprintf(dlt, "%s%s", fmt_folder, "note_for_injection_line.dlt");
    cat_left_file_append_2_right_file(dlt, dst_file_path);
}

int file_existed_for_reading(char *file_path, int line, char *src) {
	FILE *a_fp = fopen(file_path, "r");
	if (a_fp != NULL) {
		fclose(a_fp);
		return 1 ;
	}
	return 0 ;
}

void file_must_existed_for_read_2(const char *file_path, int line, char *src) {
	FILE *a_fp = fopen(file_path, "r");
	if (a_fp != NULL) {
		fclose(a_fp);
	} else {
		char msg[128] ;
		sprintf(msg, "No file existed for reading, indirectly called from %s:%d\n        Source of this msg", src, line);
		SD2( msg, file_path);
		END2 ;
	}
}
/*
void check_file_existed_ovld_1(char *file_path, char *msg) {
	file_must_existed_for_read(file_path);
}
*/

void check_file_existed_ovld_2(char *file_path, int line_number, char *src_file_name) {
	char buf[128];
	sprintf(buf, "@%s:%d",src_file_name, line_number); 	//itoa (line_number,buf,10) ;
	file_must_existed_for_read_2(file_path, line_number, src_file_name);

}

FILE * open_simple_file_to_read_and_return_a_file_pointer(char * file_path){

	FILE ** file_ptr = malloc(sizeof(FILE * ));
	if ((*file_ptr = fopen(file_path, "r")) == NULL) {
		printf("Bad Class Name: %s\n", file_path);
		exit(-1) ;
	    return (NULL);
	}
	return *file_ptr ;
}


void open_file_to_read_by_feeding_a_file_pointer(FILE ** par_pt, char * file_path){

	if ((*par_pt = fopen(file_path, "r")) == NULL) {
		printf("Exit: Bad file_path: [%s]\n", file_path);
		sD(__LINE__, __FILE__,"Bad file path");
		exit(-1) ;
	}
}

/*extern FILE * g_pt ;

void open_simple_file_to_read_using_global_file_pointer(char * file_path){
	if(0) printf("\n21 inside ext: [%p]\n", g_pt);
	if ((g_pt = fopen(file_path, "r")) == NULL) {
		printf("Exit: Bad Class Name: %s\n", file_path);
		exit(-1) ;
	}
}*/


void fatal_end(int line, char *file){

	printf("  !!!FaTaL ExCePtiOn SoUrCe @%s:%d\n", file, line);
	exit(0) ;
}

int ext_fgets(char * pbuf , int len, FILE *f_pt) {

	do{
		fgets(pbuf, len, f_pt);
	}while(pbuf[0] == '*') ;
	return(strlen(pbuf));
}


int ext_fgets_skip_new_line(char * pbuf , int len, FILE *f_pt) {
    char *get ;
	do{  //skip line begin with '*'
		get= fgets(pbuf, len, f_pt);
	}while((pbuf)[0] == '*') ;
	if(get == NULL){
		char error_message[128];
		sprintf(error_message, "No token get from file");
		SD2("error_message", error_message) ;
		END2 ;
	}
	int i=0 ;
	for(i=0; i<strlen(pbuf); i++){
		if( *(pbuf+i) == ' ')
		{
			SD2("space is not permitted in this line", pbuf);
			end2(__LINE__,__FILE__);
		}
	}
	if(pbuf[strlen(pbuf)-1]=='\n')  //to solve in case no new line in pBuf
		pbuf[strlen(pbuf)-1]='\0';
	return(strlen(pbuf));
}


void process_parameter(char **char_pt, FILE *f_pt)
{

	if(0)
	{
		int a=0 ;
		printf("\n&a is %p", &a);
		printf("\n[1] char_pt is %p", char_pt);
	}

	*char_pt = (char*)malloc(128);
    ext_fgets(*char_pt, 128, f_pt);
    *((*char_pt)+strlen(*char_pt)-1)= '\0';

    if(0){
    	int i ;
    	printf("\n");
    	for(i=0; i<strlen(*char_pt); i++)
    	printf("%c|", *(*char_pt+i)) ;
    }

    if(0) printf("\nsizeof(char_pt) is %lu", sizeof(*char_pt));
    if(0) printf("\n[2] char_pt is %p", *char_pt);
    if(0) printf("\n%s\n", *char_pt);
}

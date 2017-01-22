/*
 * helper.h
 *
 *  Created on: 2012/11/6
 *      Author: ta
 */

#ifndef HELPER_H_
#define HELPER_H_
//////////////////////////////////////////////////////////////////////////
#include <dirent.h>
#define DIR_ALREADY_EXIST  1
#define DIR_NOT_EXIST	0

int checkIfDirExisted(const char * const theDir);
//////////////////////////////////////////////////////////////////////////////

#define MAIN_FUNCTION if
#define PROC if(1)
#define ALL_BEGIN if(1)
#define ZONE if(1)
#define SKIP (if 0)

#define END end(__LINE__)
#define END2 end2(__LINE__,__FILE__)
//#define SD(tag, var) sD(__LINE__, (tag), (var))
#define SD2(tag, var) sD2(__LINE__,__FILE__,(tag), (var))

#define POP(var)	SD2( 'var', var)
void POP_FAIL(char *varID);


#define sprt_sh_cmd  	sprintf
#define string_path_or_node	sprintf

void put_line_to_lfile_head(char *file, char *line) ;
void fatal_end(int line, char *file) ;
void consGrdNodes(char *taAccount);
int  file_existed_for_reading(char *file_path, int line, char *src);
void file_must_existed_for_read_2(const char *file_path, int line, char *src);
void end(int line_number);
void end2(int, char*);
//void system_call(char *s) ;
void linux_system_cmd_dump(char *f, int line, char *s) ;
void sh_out_variable( char *cmd,  char *var);
void sD2(int lineNumber, char* fileLeaf, const char *pToken, const char *p);
void sD(int lineNumber, char *pToken, char *p);
void sfD(int lineNumber, char* filePath,  char *pToken, char *p);
//////////////////////////////////////////////////////////////////////////
int ext_fgets_skip_new_line(char * pbuf , int len, FILE *f_pt);
int  ext_fgets(char * buf , int len, FILE *f_pt);
void process_parameter(char **students_home_folder, FILE *f_pt);
FILE * open_simple_file_to_read(char * file_path);
void open_simple_file_to_read_using_global_file_pointer(char * file_path);
void open_file_to_read_by_feeding_a_file_pointer(FILE ** file_ptr, char * file_path);
FILE * open_simple_file_to_read_and_return_a_file_pointer(char * file_path);

char* read_id(char *student_id, int len, FILE* f_pt);
void  cat_left_file_append_2_right_file(char* dlt_file_name, char * file_name);
void append_left_string_2file(char* str, char * file_name);
void check_file_readable_ovld_2(char *file_path, int line_number) ;
void dlt_2_file(const char * const dlt_file_name, char * file_name, char * );
int size_of_file(char *);
void append_string_2fileS(int n, ...) ;

int isNode(char *pS);    //[/a/b/c/]
int isHat(char *pS);     //[/a/b/c]   a is root
int isBridge(char *pS);  //[a/b/c ]   a and c are folder
int isBranch(char *pS);  //[a/b/c ]   a is folder but c leaf
int isPath(char *pS);

void  junk_copy_student_grade_to_ta_and_to_his_profile();

#define COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE() \
char to_ta_per_student_graded_path[256] ;\
{\
	sprintf(to_ta_per_student_graded_path, "%s%s",ta_grade_all_folder, per_student_graded_leaf) ;\
}\
char copy_student_graded_record_to_ta_project_cmd[256];\
{\
	sprintf(copy_student_graded_record_to_ta_project_cmd,"cp %s %s", \
			per_student_grading_branch, \
			to_ta_per_student_graded_path);\
	system(copy_student_graded_record_to_ta_project_cmd);\
};\
char to_student_graded_profile_path[256];\
{\
sprintf(to_student_graded_profile_path, "%s%s",per_student_grading_profile_holder, per_student_graded_leaf) ;\
}\
char copy_student_graded_record_to_his_profile_cmd[256];\
{\
	sprintf(copy_student_graded_record_to_his_profile_cmd,"cp %s %s", \
			per_student_grading_branch, \
			to_student_graded_profile_path);\
	system(copy_student_graded_record_to_his_profile_cmd);\
};


#define EXIT_STATE_0() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();
#define EXIT_STATE_1() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();
#define EXIT_STATE_2() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();
#define EXIT_STATE_3() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();
#define EXIT_STATE_4() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();
#define EXIT_STATE_5() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();
#define EXIT_EXACT() COPY_STUDENT_GRADE_TO_TA_AND_TO_HIS_PROFILE();

#define ECHO_TO_1_FILE(OUT_STRING) 		append_left_string_2file((OUT_STRING), CPT_running_recordS_summary);

#define ECHO_TO_2_FILES(OUT_STRING)		ECHO_TO_1_FILE(OUT_STRING); \
										append_left_string_2file((OUT_STRING), per_student_grading_branch);

#define ECHO_TO_3_FILES(OUT_STRING) 	ECHO_TO_2_FILES(OUT_STRING)\
										append_left_string_2file((OUT_STRING), latency_no_sort);

#define ECHO_TO_4_FILES(OUT_STRING) 	ECHO_TO_3_FILES(OUT_STRING)\
										append_left_string_2file((OUT_STRING), exact_no_sort);

#define ECHO_NO_PROJECT_STATE(OUT_STRING) 		ECHO_TO_2_FILES(OUT_STRING) ;
#define ECHO_NO_BINARY_STATE(OUT_STRING) 		ECHO_TO_2_FILES(OUT_STRING) ;

#define ECHO_ANSWER_FILE_TOO_LARGE_STATE(OUT_STRING) ECHO_TO_3_FILES(OUT_STRING) ;
#define ECHO_NO_ANSWER_LINE_STATE(OUT_STRING) 	ECHO_TO_2_FILES(OUT_STRING) ;


#define ECHO_LONG_ANSWER_LINE_STATE(OUT_STRING) ECHO_TO_3_FILES(OUT_STRING) ;
#define ECHO_NOT_EXACT_STATE(OUT_STRING)      	ECHO_TO_3_FILES(OUT_STRING) ;
#define ECHO_EXACT_STATE(OUT_STRING) 			ECHO_TO_4_FILES(OUT_STRING) ;

#define ECHO_TO_FILES(OUT_STRING) \
										append_string_2fileS(4,\
												(OUT_STRING),\
												per_student_grading_branch,\
												latency_no_sort,\
												CPT_running_recordS_summary\
												);




#endif /* HELPER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "util.h"
//#include <stdio.h>
//#include <conio.h>
#include <sys/types.h>;
#include <dirent.h>


void update_sln(char *linked, char *sl)
{
	char sh_cmd[512];
	if(fopen(sl, "r") != NULL) {
		sprintf(sh_cmd, "rm %s", sl);
		linux_system_cmd_dump(__FILE__, __LINE__, sh_cmd);
		SD2("sh_cmd",sh_cmd);
	}
//END2 ;
	sprintf(sh_cmd, "ln -s %s %s", linked, sl);
	SD2("sh_cmd",sh_cmd);
	linux_system_cmd_dump(__FILE__, __LINE__, sh_cmd);

}
void get_id_from_branch(char *id, char *branch, int line_number, char *file_name )
{
		char *header="";
		char name[128];
		FILE *fp ;
		sprintf(name, "%s%s",header, branch);
		if((fp=fopen(name, "r"))!= NULL){  //target file is exist
			ext_fgets_skip_new_line(id , 128, fp);
		}
		else {
			//char buf[128] ;
			//sprintf(buf, "No file [%s] to acquaint the job. \n        Prepare it!", name);
			SD2("No file to read the effective job argument",name);
			end2(__LINE__,__FILE__);
		}
}

void set_id_to_leaf(char *file, char *id )
{
		//char *JOB="job/";
		//char buf[128];
		FILE *fp ;
		//sprintf(buf, "%s%s",JOB, file); //"0_students_home_folder");

		if((fp=fopen(file, "w"))!= NULL){
			fprintf(fp, "%s", id);
			fclose(fp);
		}
		else {
			//SD2("error open file",buf) ;
			//END2 ;
			end2(__LINE__,__FILE__);
		}
		SD2(id,file);
}


void extract_kcmc_from_ws2class(char *my_kcmc, char *my_ws2class)
{
	//SD2("the_ws2class",the_ws2class);
	//end2(__LINE__,__FILE__);
	char the_ws2class[128];
	strcpy(the_ws2class, my_ws2class) ;
	char *pt1=strchr(the_ws2class,(int) '/');
	char *pt2=strchr(pt1+1, (int)'/');
	char bj[128];
	char kc[128];
	strcpy(bj, pt2+1);
	SD2("bj",bj);
	*pt2='\0';
	strcpy(kc, pt1+1);;
	SD2("kc",kc);

	sprintf(my_kcmc, "%s.%s", kc, bj);
	SD2("my_kcmc", my_kcmc);
	//end2(__LINE__,__FILE__);
}


int answer_is_good(char * ta_ans_last_line, char * student_ans_last_line)
{
	return (strcmp(ta_ans_last_line, student_ans_last_line)==0);
}


void  junk_copy_student_grade_to_ta_and_to_his_profile(char *ta_grade_all_folder, char* per_student_grading_profile_holder,
												char* per_student_graded_leaf, char* per_student_grading_path )
{

	SD2("ta_grade_all_folder",ta_grade_all_folder);
	SD2("per_student_grading_profile_holder",per_student_grading_profile_holder);
	SD2("per_student_graded_leaf", per_student_graded_leaf);
	SD2("per_student_grading_path", per_student_grading_path);
	END2;

	char to_ta_per_student_graded_path[256] ;
	{
		sprintf(to_ta_per_student_graded_path, "%s%s",ta_grade_all_folder, per_student_graded_leaf) ;
	}
	copy_right_path_to_left_path(to_ta_per_student_graded_path, per_student_grading_path);
	/*
	char copy_student_graded_record_to_ta_project_cmd[256];
	{
		sprintf(copy_student_graded_record_to_ta_project_cmd,"cp %s %s",
				per_student_grading_path, //src
				to_ta_per_student_graded_path);
		linux_system_cmd_dump(__FILE__, __LINE__, copy_student_graded_record_to_ta_project_cmd);

	};
	 */
	char to_student_graded_profile_path[256];
	{
		sprintf(to_student_graded_profile_path, "%s%s",per_student_grading_profile_holder, per_student_graded_leaf) ;
	}
	copy_right_path_to_left_path(to_student_graded_profile_path, per_student_grading_path);
	/*
	char copy_student_graded_record_to_his_profile_cmd[256];
	{
		sprintf(copy_student_graded_record_to_his_profile_cmd,"cp %s %s",
				per_student_grading_path, //src
				to_student_graded_profile_path);
		linux_system_cmd_dump(__FILE__, __LINE__, copy_student_graded_record_to_his_profile_cmd);
	};
	*/
}


// http://pubs.opengroup.org/onlinepubs/007908799/xsh/readdir.html
//#include <sys/types.h>
//#include <dirent.h>

struct dirent *readdir(DIR *dirp);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

struct dirent *readdir(DIR *dirp);
//int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
struct dirent *readdir(DIR *dirp);

#define FOUND 		1
#define NOT_FOUND 	2
#define READ_ERROR 	4
#define OPEN_ERROR 	8
//#include <string.h>
int list_file_in_folder(char *path, char *job_aid)
{
	DIR *dirp;
	int errno=0;
	struct dirent *dp ;
	char cmdBuf[128];
	char target[128];
	sprintf(cmdBuf, "ls -l %s > %s",path, job_aid );
	SD2("cmdBuf", cmdBuf) ;
	linux_system_cmd_dump(__FILE__, __LINE__, cmdBuf);

	return 0 ;

	char*name ;
	dirp = opendir(path);

	while (dirp) {
	    errno = 0;
	    if ((dp = readdir(dirp)) != NULL) {
	    	SD2("dp->d_name",dp->d_name);
	        if (strcmp(dp->d_name, name) == 0) {
	            closedir(dirp);
	            return FOUND;
	        }
	    } else {
	        if (errno == 0) {
	            closedir(dirp);
	            return NOT_FOUND;
	        }
	        closedir(dirp);
	        return READ_ERROR;
	    }
	}
	return OPEN_ERROR;
}

void append_student_answer2student_grading_sheet(
		const char* per_student_grading_branch, char* cur_ID_answer_leaf) {
	{
		append_left_string_2file("@[START YOUR ANSWER]",
				per_student_grading_branch);
		cat_left_file_append_2_right_file(cur_ID_answer_leaf,
				per_student_grading_branch);
		append_left_string_2file("@[END YOUR ANSWER]",
				per_student_grading_branch);
		SD2("per_student_grading_branch", per_student_grading_branch);
		//cjchng END2 ;
	}
	return;
}





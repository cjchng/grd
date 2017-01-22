/*
 ============================================================================
 Name        : homework.c
 Author      : cj-chang
 Version     : change to single project grading
 Copyright   : Your copyright notice
 Description : C class aid program
 	 	 	 (1)extending latency index to grade file
 	 	 	 (2)limit and set execution time of student project to 1 second.
 	 	 	 2012-11-07
 	 	 	 modify gnu compatible type of timespec
 using: cmd class_name session_name project_name
 ============================================================================
 */
/*
 * Naming convention
 * xxx_path:long filename
 * xxx_name:short filename;
 * xxx_bridge: intermediate routing node not including root and leaf
 * XXX_folder: root extending bridge
 *
 */

/***
 * 13Sep:
 * solution_account_nfs_node extents
 * to let the invocation from any client
 */

/*
//http://stackoverflow.com/questions/2387485/limiting-the-time-a-program-runs-in-linux

 ./myProgram &
sleep 1
kill $! 2>/dev/null && echo "myProgram didn't finish"
 *
 *
 */
//examples of run configuration
//-n 2013-C-yi-wk01-fig02_04


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>

#include "like.h"
#include "helper.h"
#include "util.h"


char edit_tool[]= "gedit ";
/*
//refer <sys/time.h>sprt_ta_path
struct timespec
{
	long int tv_sec;
	long int tv_usec;
};
 */

#define gnu_time_t struct timeval
//#define gnu_time_t struct timespec

gnu_time_t get_modify_time(char *path, struct stat stat_buf,char *date,  int * flag);

struct stat student_exe_stat_buf ;
//////////////////////
char copy_right_logo[]=" .............MCUT OIL_LAB, 2012-2014. Copy Right Reserved............";

//char ta_nfs_account_node[128] ;
//char ta_account[128] ;
const int OBSOLETE = 1 ;
const int IS_A_SCOPE = 1 ;

//char fmt[]="fmt/";
//char ANS[]="ANS/";
//char TA[]="TA/" ;

//char ftp_folder[] = "/home/ta/ftp/";         //2012-1021 check ftp_folder

//char TA_NODE[] = "/home/ta/";				//where dwelled this execution file
char RUN_OR_DEBUG[] = "/Debug/";

char grading_node[256] ;					/// this node is for each project under graders working environment
//char fmt_node[256];
char name_list_node[256]="asset/name_list/";
// move to support extern in help.c
//char per_project_graded_answer_node[256];    ///this node stores graded results of the target namelist group

char ta_solution_node[256];

/*
void putOutComeHat(char *pNode)
{
	char aBuf[256] ;
	strcpy(aBuf,GRADED_RESULT_HAT );    //some day the graded result will store in a place such as NAS node.
	strcat(aBuf, pNode);
	strcpy(pNode, aBuf)	;
}
 */
const int NOT_ONLY_FOR_TA=0 ;


void open_file_for_write( char *path)
{
	FILE *fp_1107;
	{
		if((fp_1107=fopen(path, "w")) == NULL){
			printf("\n{%s}\nFile open error for writting\n ", path);
			exit(-1);
		}
		else fclose(fp_1107) ;
	}
}

const char *FMT_NODE= "asset/fmt/";
char *cons_fmt_node()
{
	return FMT_NODE ;
}


void new_a_file_with_caption(char *file_path, char *copy_right)
{
	open_file_for_write(file_path);
	append_left_string_2file(copy_right, file_path);
}



void head_of_grading_path(char *path, char *msg)
{
	char aFmt[256];
	strcpy(aFmt, cons_fmt_node());
	strcat(aFmt, "at_line.dlt");
	SD2("aFmt", aFmt) ;
	cat_left_file_append_2_right_file(aFmt, path);
	append_left_string_2file(msg, path);
	strcpy(aFmt, cons_fmt_node());
	strcat(aFmt, "plus_line.dlt");
	cat_left_file_append_2_right_file(aFmt, path);
}

void pack_latency_common_msg(char *student_latency_msg, long int latency, char *student_date, char *student_id)
{
	sprintf(student_latency_msg, "%+08ld Index,[%s] ID:[%9s]", latency, student_date, student_id);
}

void append_right_path_to_left_path(char *dst_path, char *org_path)
{
	char per_project_shell_cmd[512];    //if(0)org_path
	{
		//copy student answer file to ta answer file of this class for instructor's visual inspection
		{
			//Beautify the output format
			append_left_string_2file("", dst_path);
			append_left_string_2file("", dst_path);
		}
		sprt_sh_cmd(per_project_shell_cmd, "cat %s >>%s", org_path, dst_path);
		linux_system_cmd_dump(__FILE__, __LINE__, per_project_shell_cmd);
	}
}

void copy_right_path_to_left_path(char *dst_path, char *org_path)
{
	//if (isPath(dst_path) && isPath(org_path))
	{
		char per_project_shell_cmd[512];    //if(0)org_path
		{
			sprt_sh_cmd(per_project_shell_cmd, "cp %s %s", org_path, dst_path);
			linux_system_cmd_dump(__FILE__, __LINE__, per_project_shell_cmd);
		}
	}
	/*else {
		fatal_end(__LINE__, __FILE__) ;
	}
*/
}



//FILE * g_pt ;
//char job_ID[128];

char *exn_pt ;

const char *CPT_ans_join_to_TA_solutions_set_path ;

void copy_ta_src_to_gradind_file(const char *const fAppendTaSourceToStudentProfile,
			const char* const per_student_grading_branch,
			const char* const CPT_ta_c_source_path) {

	SD2("fAppendTaSourceToStudentProfile", fAppendTaSourceToStudentProfile) ;
	SD2("per_student_grading_branch",per_student_grading_branch) ;
	SD2("CPT_ta_c_source_path",CPT_ta_c_source_path);
	//END2 ;
	if (strcmp(fAppendTaSourceToStudentProfile, "true") == 0) { //Append TA's C Source Code to Student's Solution Files
		char copy_cmd[256];
		{ //delemeter appended
			append_left_string_2file("", per_student_grading_branch);
			append_left_string_2file(".1....................................",
					per_student_grading_branch);
			append_left_string_2file(".2....Solution Code From Instructor....",
					per_student_grading_branch);
			append_left_string_2file(".3....................................",
					per_student_grading_branch);
			append_left_string_2file("", per_student_grading_branch);
		}

		sprt_sh_cmd(copy_cmd, "cat %s >>%s 2>> %s", CPT_ta_c_source_path,
				per_student_grading_branch, per_student_grading_branch);
		SD2("copy_cmd",copy_cmd);
		linux_system_cmd_dump(__FILE__, __LINE__, copy_cmd); //transmit C source to student 2012-1106
		//END2 ;
	}
	//return cmd_buf;
}

typedef struct job_id_2_branch{
	char *job_id;
	char *its_branch ;
	void (*job) (char *job_id, char * branch, int, char*); // get_job_id_from_branch;
} job_id_2_branch_t;




//MAIN_FUNCTION;
int main(int argc, char **argv) {


	/*
	PROC;
	char ta_nfs_folder[128];
	char ta_account[256];
	char ws2class[128] ;
	char chapter_session[128];
	char graded_project[128];
	char kcmc[128] ;
	char injection_tag[128];
	char studentS_home_folder[128];

	{
		char *job_seg[]={
						 "job/5..ta_nfs_node",
						 "job/4..ta_account",
					     "job/3..ws2class",
					     "job/2..chapter_session",
					     "job/1..graded_project",
					     "x.kcmc.GET",			//name list
					     "y.injection_tag"} ;

		char *cntxt_seg[]={
					ta_nfs_folder,    //temp
					ta_account,
					ws2class,
					chapter_session,
					graded_project,
					kcmc,
					injection_tag
				} ;
//END2;
		int i;
		for(i=0; i<5; i++)
		{
			//get_id_from_branch(fAppendTaSourceToStudentProfile, "misc/AppendTaSourceToStudentProfile", __LINE__, __FILE__) ;
			get_id_from_branch(cntxt_seg[i], job_seg[i], __LINE__, __FILE__) ;
		}
		PROC;  //get
		{
			get_id_from_branch(studentS_home_folder, "job/6..studentS_home_folder", __LINE__, __FILE__) ;
			SD2("studentS_home_folder",studentS_home_folder) ;
			get_id_from_branch(injection_tag, "job/0..injection_tag", __LINE__, __FILE__) ;
			SD2("injection_tag",injection_tag) ;
		}

	}
	job_id_2_branch_t job_config= {ta_nfs_folder, "job/5..ta_nfs_node",  get_id_from_branch};
	job_config.job(job_config.job_id, job_config.its_branch, __LINE__, __FILE__ ) ;
	SD2("ta_nfs_folder", ta_nfs_folder) ;
	*/

	PROC;
		char ta_nfs_folder[128];
		char ta_account[256];
		char ws2class[128] ;
		char chapter_session[128];
		char graded_project[128];
		char kcmc[128] ;
		char injection_tag[128];
		char studentS_home_folder[128];
		{
		job_id_2_branch_t MY_JOB[] = {
				{injection_tag, 	"job/0..injection_tag", 	get_id_from_branch},
				{graded_project, 	"job/1..graded_project",	get_id_from_branch},
				{chapter_session,	"job/2..chapter_session",	get_id_from_branch},
				{ws2class,			"job/3..ws2class",			get_id_from_branch},
				{ta_account,    	"job/4..ta_account",		get_id_from_branch},
				{ta_nfs_folder, 	"job/5..ta_nfs_node",		get_id_from_branch},
				{studentS_home_folder, "job/6..studentS_home_folder",get_id_from_branch}

			};
		//printf("length = %l\n", sizeof(MY_JOB)/sizeof(*MY_JOB)) ;
		{
			FILE *fp ;
			if((fp =fopen("misc/job_config", "w")) != NULL) fclose(fp);
			else fatal_end(__LINE__, __FILE__) ;

			int i ;
			for(i=0; i<sizeof(MY_JOB)/sizeof(*MY_JOB); i++){
				MY_JOB[i].job(MY_JOB[i].job_id, MY_JOB[i].its_branch, __LINE__, __FILE__ ) ;
				append_left_string_2file("*******************", "misc/job_config") ;
				append_left_string_2file(MY_JOB[i].its_branch, "misc/job_config") ;
				append_left_string_2file(MY_JOB[i].job_id, "misc/job_config") ;
			}
		}
	}


//END2 ;
	char ta_account_node[128] ;
	char *pt_workspace_class_bridge_of_ta = ws2class;   // "workspace/C-2013[course]/jia[class]" ;
	{

		PROC; //get ta_nfs_account_node
		{
			SD2("ta_account", ta_account);
			sprintf(ta_account_node, "%s%s/",ta_nfs_folder, ta_account);
			SD2("ta_nfs_account_node",ta_account_node);
		}
		//END2;
		PROC; //show section aid
		{
			char chapter_session_AID_cmd[128];
			sprintf(chapter_session_AID_cmd, "%s%s%s%s", "ls -l ", ta_account_node, ws2class, " > job/2_chapter_session.AID");
			SD2("chapter_session_AID_cmd", chapter_session_AID_cmd);
			linux_system_cmd_dump(__FILE__, __LINE__, chapter_session_AID_cmd);
		}
		//END2 ;
		PROC; //show project aid
		{
			char graded_project_AID_cmd[128] ;
			sprintf(graded_project_AID_cmd, "%s%s%s/%s%s", "ls -l ", ta_account_node, ws2class,chapter_session, " > job/1_graded_project.AID");
			SD2("graded_project_AID_cmd", graded_project_AID_cmd);
			linux_system_cmd_dump(__FILE__, __LINE__, graded_project_AID_cmd);
		}
		//END2 ;
		PROC; //check existed ta_project_node
		{
			char ta_project_node[256];
			//home/ta/workspace/jia/wk01-c2/fig02_01
			sprintf(ta_project_node,"%s%s/%s/%s/%s",
					ta_nfs_folder,ta_account,ws2class,chapter_session,graded_project);
			if( checkIfDirExisted(ta_project_node) == 0) {
				SD2("ta_project_node",ta_project_node);
				SD2("TA PROJECT IS NOT EXISTED", "Please configure them!");
				END2;
			}
		}

	}

	char fAppendTaSourceToStudentProfile[128];
	//get_id_from_job_leaf(injection_tag, "0..injection_tag") ;
	get_id_from_branch(fAppendTaSourceToStudentProfile, "misc/AppendTaSourceToStudentProfile", __LINE__, __FILE__) ;
	SD2("fAppendTaSourceToStudentProfile", fAppendTaSourceToStudentProfile) ;
	//END2 ;
	/*PROC;  //get
	char studentS_home_folder[128];
	{
		get_id_from_branch(studentS_home_folder, "job/6..studentS_home_folder", __LINE__, __FILE__) ;
		SD2("studentS_home_folder",studentS_home_folder) ;
		get_id_from_branch(injection_tag, "job/0..injection_tag", __LINE__, __FILE__) ;
		SD2("injection_tag",injection_tag) ;
	}*/

	PROC; //this is a specific implementation
	{
		extract_kcmc_from_ws2class(kcmc, ws2class);
		printf("..studentS_home_folder: %s\n",studentS_home_folder );
		printf("..kcmc: %s\n",kcmc);
		printf("..ws2class: %s\n", ws2class);
		printf("..chapter_session: %s\n",chapter_session );
		printf("..graded_project: %s\n", graded_project);
		printf("..injection_tag: %s\n", injection_tag );
	}

	PROC; //make the configured directory :Get/
	{
		/*/////////////////////////////////////////////////////////////////////////
		#include <dirent.h>
		#define DIR_ALREADY_EXIST  1
		#define DIR_NOT_EXIST	0
		int checkIfDirExisted(char *theDir);
		 *//////////////////////////////////////////////////////////////////////////////
		if(checkIfDirExisted(GET_LIKE_H)== DIR_NOT_EXIST)
		{
			char cmd[128];
			sprintf(cmd,"mkdir Get");
			linux_system_cmd_dump(__FILE__, __LINE__, cmd);
		}
	}

	PROC; //Update "Get/kcmc", capable UI modification
	{
		char get_kcmc[128] ;
		sprintf(get_kcmc,"%s/%s", GET_LIKE_H, KCMC_LIKE_H);
		set_id_to_leaf(get_kcmc, kcmc);
		SD2("get_kcmc", get_kcmc);
		SD2("kcmc", kcmc);
	}

	////////////////////////////////////////////////////////////////////
	consGrdNodes(ta_account) ;
	////////////////////////////////////////////////////////////////////

	// http://www.lemoda.net/c/directory/
	PROC; char *current_worked_dir;
	{
		#include <errno.h>
		{

			current_worked_dir = getcwd (0, 0);
			if (! current_worked_dir) {
				fprintf (stderr, "getcwd failed: %s\n", strerror (errno));
			} else {
				printf ("%s\n", current_worked_dir);
				//free (cwd);
			}
		}
	}
	SD2("current_worked_dir", current_worked_dir);

	PROC;
	//char s_student_list_path[256] ; //don't update this field
	const char * CPT_student_list_path;
	{//////////////////////////////////////////////////////////////
		static char s_student_list_path[256] ; //don't update this field
		CPT_student_list_path= s_student_list_path ;
		sprintf(CPT_student_list_path, "%s/%s%s",current_worked_dir, name_list_node, kcmc);
	//////////////////////////////////////////////////////////////
	}
	SD2("CPT_student_list_path",CPT_student_list_path);
    //END2 ;
	PROC; //symbol link CPT_student_list_path to get.student_list
	file_must_existed_for_read_2(CPT_student_list_path, __LINE__, __FILE__);
	{
		char getDstudent_list[128];
		sprintf(getDstudent_list, "%s/%s", GET_LIKE_H, STUDENTS_LIST_LIKE_H); // = "Get/students-list" ;
		if(file_existed_for_reading(getDstudent_list, __LINE__, __FILE__ ))
		{//remove symbol link, otherwise, the next block can't make this symbol to link  CPT_student_list_path
			char rm_get_students_list_cmd[128] ;
			sprintf(rm_get_students_list_cmd, "rm %s", getDstudent_list );
			linux_system_cmd_dump(__FILE__, __LINE__, rm_get_students_list_cmd);   //otherwise the new symbols can not start
		}

		char student_exe_ln_s[128];
		sprintf(student_exe_ln_s, "ln -s %s %s", CPT_student_list_path, getDstudent_list) ;
		SD2("student_exe_ln_s",student_exe_ln_s);
		linux_system_cmd_dump(__FILE__, __LINE__, student_exe_ln_s) ;
	}
	//END2 ;
	PROC;

	const char * CPT_runner_node;
	{
		static char runner_node[128] ;
		CPT_runner_node=runner_node;
		//sh_out_variable( "echo $HOME",  runer_node_open);   //strange error
		char my_account[128] ;
		sh_out_variable( "whoami ",  my_account);
		sprintf(runner_node, "/home/%s/",my_account ) ;

	}
	SD2("cpt_runner_node", CPT_runner_node);


	PROC;
	FILE **name_list__rptr=NULL;
	{
		file_must_existed_for_read_2(CPT_student_list_path, __LINE__, __FILE__);
		name_list__rptr = fopen(CPT_student_list_path,"r");
	}
//END2 ;
	PROC;
	const char * CPT_out_project_grading_seg ;
	char ta_project_folder[256];
	const char * const CPT_ta_project_folder= ta_project_folder ;
	char injections_y_source_code_path[256];
	const char * const CPT_injections_y_source_code_path = injections_y_source_code_path ;
	{
		char ta_course_folder[128];
		const char * const CPT_ta_course_folder= ta_course_folder;

		string_path_or_node(ta_course_folder, "%s%s/", ta_account_node, ws2class);     //2012-1021
		SD2("CPT_ta_course_folder", CPT_ta_course_folder);
		//====process for all==========================================================
		string_path_or_node(ta_c_source_path, "%s%s/%s/src/%s.c", CPT_ta_course_folder, chapter_session, graded_project, graded_project); //.c =>c .cpp=>c++
		SD2("CPT_ta_c_source_path",CPT_ta_c_source_path);
		//else
		//	cons_ta_c_source_path(ta_c_source_path);
		//====process for all==========================================================

		file_must_existed_for_read_2(CPT_ta_c_source_path, __LINE__, __FILE__);   //if existed the C source code of account ta
		{
			if(NOT_ONLY_FOR_TA ==0 || NOT_ONLY_FOR_TA == 1){
				string_path_or_node(ta_project_folder, "%s%s/%s/%s/", ta_account_node,ws2class, chapter_session, graded_project);
				SD2("CPT_ta_project_folder",CPT_ta_project_folder);
			}
			///////////////////////////////////////////////////////////////////////////
			//{//redirect to the working directory for easy packaging
			//END2 ;
			static char out_project_grading_seg[128] ;
			CPT_out_project_grading_seg = out_project_grading_seg ;
			string_path_or_node(CPT_out_project_grading_seg,"%s.%s.%s/%s@%s/", OUT_ASM_LIKE_H,chapter_session,kcmc, graded_project, injection_tag); //argv[2] is configure file

			SD2("out_project_grading_seg", CPT_out_project_grading_seg);
//END2 ;
			string_path_or_node(injections_y_source_code_path, "%s%s", CPT_out_project_grading_seg, "injection_y_source_code");
			SD2("CPT_injections_y_source_code_path",CPT_injections_y_source_code_path) ;
//END2 ;
			if(!checkIfDirExisted(CPT_out_project_grading_seg))
			{   //the folder is not existed, can be used to synchronize the ta answering time
				char make_project_grading_folder_cmd[128];
				sprt_sh_cmd(make_project_grading_folder_cmd,"mkdir -p %s",CPT_out_project_grading_seg);
				SD2( "project_grading_folder",CPT_out_project_grading_seg);
				linux_system_cmd_dump(__FILE__, __LINE__, make_project_grading_folder_cmd) ;
				SD2( "make_project_grading_folder_cmd", make_project_grading_folder_cmd);
//END2 ;
				FILE * inject_fp;
				{   //create injection file
					if ((inject_fp = fopen(CPT_injections_y_source_code_path, "w")) != NULL) {
						fclose (inject_fp) ; }
					else { fatal_end(__LINE__, __FILE__); } ;

					//Copy C source code for guiding grader to prepare testing injections
					dlt_2_file( CPT_injections_y_source_code_path, cons_fmt_node(), "note_for_injection_line.dlt"  );
					cat_left_file_append_2_right_file(CPT_ta_c_source_path, CPT_injections_y_source_code_path);
					dlt_2_file(CPT_injections_y_source_code_path, cons_fmt_node(), CPT_ta_c_source_path );
					//configure the testing input
					char sh_edit_injecting_file[256]; //edit injection file
					sprt_sh_cmd(sh_edit_injecting_file, "%s %s",edit_tool, CPT_injections_y_source_code_path);
					SD2("sh_edit_injecting_file",sh_edit_injecting_file);
					linux_system_cmd_dump(__FILE__, __LINE__, sh_edit_injecting_file);
				}
			}  //end of prepare injection_y_source
		}
	}

	//END2 ;

	///////////////////////////////////////////////////////////
	PROC;
	{
	//update_sln(char *linked, char *sl)
	char ta_project_name[256] ;
	sprintf(ta_project_name, "%s.%s", "./ta-project", graded_project);
	SD2("ta_project_name", ta_project_name);
	update_sln( CPT_ta_project_folder, ta_project_name);
	//update_sln( CPT_ta_project_folder, "./ta-project");
	SD2("CPT_ta_project_folder", CPT_ta_project_folder);
	//Value --> /mnt/c-ta-2013/ta/workspace/C-2013/jia/wk06-c5/fig05_12/
	//END2 ;
	}
	//////////////////////////////////////////////////////////////////////////

	file_must_existed_for_read_2(CPT_injections_y_source_code_path, __LINE__, __FILE__);
	SD2("CPT_injections_and_source_code_path",CPT_injections_y_source_code_path);


	const char * CPT_all_ID_grading_sheet_branch;
	{
		static char all_ID_grading_sheet_branch[256];
		CPT_all_ID_grading_sheet_branch =  all_ID_grading_sheet_branch ;
		{//redirect the last statement for debugging
			string_path_or_node(all_ID_grading_sheet_branch, "%s{ans-from-students}{%s}@{%s}", CPT_out_project_grading_seg, graded_project, injection_tag);
			SD2("CPT_all_ID_grading_sheet_branch", CPT_all_ID_grading_sheet_branch) ;
		}
		{
			{
				char buf[256];
				sprintf(buf, "echo %s > Get/assessment", CPT_out_project_grading_seg);
				linux_system_cmd_dump(__FILE__, __LINE__, buf);
				SD2("project_grading_folder", CPT_out_project_grading_seg) ;
			}

			{
				FILE *fpt;
				if ((fpt = fopen(CPT_all_ID_grading_sheet_branch, "w")) == NULL){
					printf("cannot open %s for grading", CPT_all_ID_grading_sheet_branch);
					fatal_end(__LINE__, __FILE__);
				}
				else
					fclose(fpt);
			}
		}
	}

//END2 ;
	/////////////////////////////////////////////////////////////////

	PROC; const char *CPT_project_grading_all_students_node ;
	{//redirect to the working directory for debugging
		static char project_grading_all_students_node[128] ;
		CPT_project_grading_all_students_node = project_grading_all_students_node ;

		string_path_or_node(CPT_project_grading_all_students_node,"%s%s", CPT_out_project_grading_seg,"all-students/");
		SD2("project_grading_all_store_folder",CPT_project_grading_all_students_node);
		//Value --> Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/all-students/

		if(!checkIfDirExisted(CPT_project_grading_all_students_node) )
		{
			char make_work_forder_cmd[128];
			sprt_sh_cmd(make_work_forder_cmd,"mkdir %s",CPT_project_grading_all_students_node);
			linux_system_cmd_dump( __FILE__, __LINE__, make_work_forder_cmd) ;
			SD2("make_work_forder_cmd", make_work_forder_cmd);
			// -: mkdir 2013-C.jia.wk01.hello_grade/all/
		}
		//END2 ;
	}


	PROC; const char *CPT_ta_solutions_path ;   //TA solution file

	if(0) //0104
	{
		static char ans_join_to_TA_solutions_set_path[256];
		CPT_ta_solutions_path = ans_join_to_TA_solutions_set_path ;
		//char ta_solution_node[256];
		//this node is very diverse
		SD2("ta_solution_node",ta_solution_node);
		//END2 ;
		{
			char mkdir_ta_solution_node[128] ;
			sprintf(mkdir_ta_solution_node ,"mkdir -p %s", ta_solution_node) ;
			linux_system_cmd_dump(__FILE__, __LINE__, mkdir_ta_solution_node) ;
		}
		SD2("ta_solution_node",ta_solution_node);
		END2 ;

		sprintf(CPT_ta_solutions_path, "%s%s.%s.%s-%s.ans",
				ta_solution_node,
				kcmc,
				chapter_session,
				graded_project,injection_tag);    //2012-1021 check ftp_folder
		SD2("kcmc",kcmc) ;
		SD2("ans_join_to_TA_solutions_set_path",CPT_ta_solutions_path) ;

		FILE *pt;
		if ((pt = fopen(CPT_ta_solutions_path, "w")) == NULL) {
			puts("Open ""ans_join_to_solutions_set_path"" file writing error!");
			fatal_end(__LINE__, __FILE__) ;
		} else {
			fclose(pt);
		}

		SD2("CPT_ans_join_to_TA_solutions_set_path", CPT_ta_solutions_path);
    }
	//END2; //
	//CPT_ans_join_to_TA_solutions_set_path  =  per_student_grading_branch ;
	//END2 ;
	/////////////////////////////////////////////
	//gnu_time_t get_modify_time(char *path, struct stat file_stat_buf)
	PROC ;
	const char * CPT_ta_exe_cmd_path;  //refactor to static
	gnu_time_t ta_last_access_time ;
	{

		static char ta_exe_cmd_path[256] ;
		CPT_ta_exe_cmd_path = ta_exe_cmd_path ;

		struct stat file_stat_buf ;
		string_path_or_node(CPT_ta_exe_cmd_path, "%s%s/%s/%s%s%s",
				ta_account_node, 			  //nfs_node_link_to_ta_account
				pt_workspace_class_bridge_of_ta,  //i.e. 2013-C.jia   "workspace/C-2013/jia" ;
				chapter_session, graded_project,  //i.e. wk02-c1, fig02_01
				RUN_OR_DEBUG ,					  //check run mode or debug mode
				graded_project);				  //execution file is equal to project name

		{
			int file_existed=1 ;
			char ta_date[256];
			ta_last_access_time=get_modify_time(CPT_ta_exe_cmd_path, file_stat_buf, ta_date, &file_existed) ;
			//another group which has no execution file
			if(file_existed ==0) {
				printf("\nta has no execution file, please provide by running it!\n") ;
				fatal_end(__LINE__, __FILE__) ;
			}
		}
		{
			char aBuf[256] ;
			sprintf(aBuf, "%ld", ta_last_access_time.tv_sec );
			SD2("ta_last_access_time.tv_sec",aBuf) ;
		}
	}
	//////////////////////////////////////////////////////
	//file_must_existed_for_read_2(ta_exe_path, __LINE__, __FILE__);
//END2 ;
	/*char *cpt;
	{
		static char a[100];
		cpt=a ;
	}*/

	/*
	#define a(x) sin(x) ;

	#define TO_STAIC_CPT(ssss, LENGTH, CPT) \
		const char * CPT \
		{{ \
	    		static char ssss [ LENGTH ]; \
	    		CPT = ssss ; \


	TO_STAIC_CPT(abc, 100, cpt) ;
	cpt = abc ;
	}}
    */

	//TO_STAIC_CPT(ta_solution_branch_2, 512, cpt) ; }

	//END2 ;

	PROC;
	const char *CPT_out_ta_solution_branch ;
	{
		static char ta_solution_branch[128] ;
		CPT_out_ta_solution_branch = ta_solution_branch ;  //if not this statement, error jump to line 419

		sprintf(CPT_out_ta_solution_branch, "%s%s",CPT_out_project_grading_seg, "ta-solution_leaf") ;

		SD2("project_grading_seg",CPT_out_project_grading_seg);
		SD2("ta_solution_branch",CPT_out_ta_solution_branch);
        //Value --> Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/ta-solution_leaf
		//END2 ;
		char using_injection_to_run_ta_binary_and_write_answeing_to_file[256];
		{
			sprt_sh_cmd(using_injection_to_run_ta_binary_and_write_answeing_to_file, "%s <%s >%s",
					CPT_ta_exe_cmd_path,  //the grader will run ta execution file
					CPT_injections_y_source_code_path,
					CPT_out_ta_solution_branch);     //2012-1021 ta_homedir check
			SD2("using_injection_to_run_ta_binary_and_write_answeing_to_file", using_injection_to_run_ta_binary_and_write_answeing_to_file);

			linux_system_cmd_dump(__FILE__, __LINE__, using_injection_to_run_ta_binary_and_write_answeing_to_file); //will update the solution file @TA folder. 2012-11-06

			SD2("CPT_ta_exe_cmd_path",CPT_ta_exe_cmd_path);
			SD2("CPT_out_ta_solution_branch",CPT_out_ta_solution_branch);
			//END2 ;
		}
		if(0) //0104 (OBSOLETE) //we satisfy with the working model, and intend to cut the pseudoFTP branch later
		{//copy ta_execution_output to ans_join_to_TA_solutions_set_path
			SD2("ans_join_to_TA_solutions_set_path",CPT_ta_solutions_path);
			append_left_string_2file("@[TA SOLUTION starts!]", 	CPT_ta_solutions_path);
			cat_left_file_append_2_right_file( CPT_out_ta_solution_branch, CPT_ta_solutions_path);
			append_left_string_2file("@[TA SOLUTION ends.]", CPT_ta_solutions_path);
			SD2("CPT_ans_join_to_TA_solutions_set_path",CPT_ta_solutions_path);
			//Value --> ./asset/pseudoFTP/grading/ta-gives-ans/C-2013.jia.wk06-c5.fig05_12-1220_e.ans
			//END2;
		}
	}
//END2 ;
	PROC; char ta_ans_last_line[1028] ;
	{
		FILE * fp_ta_leaf ;
		if((fp_ta_leaf=fopen(CPT_out_ta_solution_branch, "r")) != NULL){
			if(fp_ta_leaf==NULL){
				SD2("(cannot open->)ta_solution_branch",CPT_out_ta_solution_branch);
				fatal_end(__LINE__, __FILE__) ;
			}

			char  line[1028];
			{ //be careful last line can not longer than 1028
				while (fgets(line, sizeof(line), fp_ta_leaf) != NULL)
					printf("%s\n", line);
			}
			strcpy(ta_ans_last_line,line);
			SD2("ta_ans_last_line", ta_ans_last_line) ;

			int ai;
			for(ai=0; ai < strlen(ta_ans_last_line); ai++)
			{
				printf("[%d]%c", ai, ta_ans_last_line[ai]);

				if(ai>1028){
					printf("TOO LONG ANS FROM TA");
					fatal_end(__LINE__, __FILE__) ; ;
				}
			}
			SD2("ta_ans_last_line",ta_ans_last_line);
			//END2 ;
		}
	}
//END2 ;
	PROC; //note header for CPT_ans_join_to_TA_solutions_set_path
	if(0) //0104 (OBSOLETE) //ditto
	{
		char aFmt[256];
		strcpy(aFmt, cons_fmt_node());
		strcat(aFmt, "minus_line.dlt");
		cat_left_file_append_2_right_file(aFmt, CPT_ta_solutions_path);
		//x_cat_dlt_by_file("~/ftp/grading/fmt/minus_line.dlt", ans_join_to_TA_solutions_set_path);
		SD2("aFmt",aFmt);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//open and clean temp file for processing grading levels
	PROC; //
	char latency_no_sort[256] ;
	{//open or clear latency_no_sort_path
		sprintf(latency_no_sort,"%s%s", CPT_out_project_grading_seg,"latency.no_sort");
		open_file_for_write(latency_no_sort) ;
		SD2("latency_no_sort", latency_no_sort);
		// Value --> Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/latency.no_sort
		//END2 ;
	}

	PROC; char exact_no_sort[256];
	{//absent_branch in this project
		sprintf(exact_no_sort,"%s%s", CPT_out_project_grading_seg,"exact.no_sort");
		open_file_for_write(exact_no_sort);

		SD2("exact_no_sort",exact_no_sort);
		//Value --> Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/exact.no_sor

		//END2 ;
	}

	PROC;
	const char * CPT_CHAPTER_SESSION_GRADING_RECORD_FOLDER ;
	//0104 const char* DOT_SUMMARY;
	//char grading_node[256] ;
	{
		static char chapter_session_grading_record_folder[256] ;
		CPT_CHAPTER_SESSION_GRADING_RECORD_FOLDER= chapter_session_grading_record_folder ;
		//***********************************************************************************************
		PROC;//(cons the chapter section folder to keep record summary
		{
			char mkdir_cmd[256];
			sprintf(chapter_session_grading_record_folder, "%s%s/%s/",grading_node,kcmc,chapter_session);
			sprt_sh_cmd(mkdir_cmd, "mkdir -p %s", CPT_CHAPTER_SESSION_GRADING_RECORD_FOLDER );
			linux_system_cmd_dump( __FILE__, __LINE__, mkdir_cmd);
			SD2("mkdir_cmd", mkdir_cmd);
			//Value --> mkdir -p ./asset/pseudoFTP/grading/C-2013.jia/wk06-c5/

		}
		//***********************************************************************************************
		/*0104 PROC;
		{//absent_branch in this project
			static char g_brief_of_this_project[256];
			DOT_SUMMARY = g_brief_of_this_project;

			sprintf(DOT_SUMMARY, "%s%s.summary",CPT_CHAPTER_SESSION_GRADING_RECORD_FOLDER, graded_project);
			new_a_file_with_caption(DOT_SUMMARY, copy_right_logo);
		}*/
		//END2 ;
	}


	const char *CPT_ta_latency_msg;
	{//ta solution lastline
		static char ta_latency_msg[256];
		CPT_ta_latency_msg = ta_latency_msg ;

		sprintf(CPT_ta_latency_msg, "%8s Index,[%24s] ID:[%s],[ANS]:%s", "ta ta ta", "     ", "  exact  ",ta_ans_last_line );
		SD2("CPT_ta_latency_msg",CPT_ta_latency_msg );
		//END2 ;
	}

	/*if(OBSOLETE)
	{
		append_left_string_2file(CPT_ta_latency_msg, DOT_SUMMARY) ;
		SD2("CPT_BRIEF_OF_THIS_PROJECT",DOT_SUMMARY );
		//END2 ;
	}*/

	//***********************************************************************************************
	//***********************************************************************************************
	//***********************************************************************************************
	//leaving blank lines
	//
	//will begin to process students, grading
	//
	//leaving blank lines
	//
	//
	//
	//***********************************************************************************************
	//***********************************************************************************************
	//***********************************************************************************************

	PROC;
	const char *CPT_absent_branch;
	const char *CPT_no_exe_branch;
	{
		{//CPT_absent_branch in this project
			static char absent_branch[256];
			CPT_absent_branch = absent_branch ;

			sprintf(CPT_absent_branch, "%s%s",CPT_out_project_grading_seg, "absent");
			new_a_file_with_caption(CPT_absent_branch, copy_right_logo);
			SD2("CPT_absent_branch",CPT_absent_branch);
		}

		{//no execution file in this project
			static char no_exe_branch[256];
			CPT_no_exe_branch = no_exe_branch ;
			sprintf(CPT_no_exe_branch, "%s%s",CPT_out_project_grading_seg,"no_exe");
			new_a_file_with_caption(CPT_no_exe_branch, copy_right_logo);
			SD2("no_exe_branch",CPT_no_exe_branch);
		}
	}
	//END2 ;
	//////////////////////////////////////////////////////////////////

	PROC;
	const char * CPT_running_recordS_summary ;
	{
		static char running_recordS_summary[256] ;
		CPT_running_recordS_summary = running_recordS_summary ;
		//open or clear latency_no_sort_path
		FILE *fp;
		//sprintf(latency_no_sort_path, "%slatency.no_sort",solution_account_folder);
		sprintf(CPT_running_recordS_summary,"%s%s", CPT_out_project_grading_seg, "running-records-summary");

		new_a_file_with_caption(CPT_running_recordS_summary, copy_right_logo);
		/*
		if((fp=fopen(CPT_running_recordS_summary, "w")) == NULL){
			printf("record.running file open for writing error");
			fatal_end(__LINE__, __FILE__);
		}
		else fclose(fp) ;
		*/
		SD2("CPT_running_recordS_summary",CPT_running_recordS_summary);
	}
////////////////////////////////////////////////////////////////////////////////////
	/*////////////////////////////////////////////////*
		PROC; //Naming student echo
		const char * CPT_per_student_graded_leaf ;
		{
			static char s_per_student_graded_leaf[256];
			CPT_per_student_graded_leaf = s_per_student_graded_leaf ;
			sprintf(s_per_student_graded_leaf,"{_%9s_}-echo-%s",student_id, graded_project) ;
			SD2("s_per_student_graded_leaf", s_per_student_graded_leaf);
			CPT_per_student_graded_leaf = s_per_student_graded_leaf ;
			//END2 ;
		}

		PROC; //Naming used in many places
		const char *per_student_grading_branch;
		{
			//char per_student_grading_branch[256];
			static char static_per_student_grading_branch[256];
			per_student_grading_branch = static_per_student_grading_branch ;
			string_path_or_node(per_student_grading_branch,"%s%s", CPT_project_grading_all_students_node, CPT_per_student_graded_leaf) ;
			SD2("per_student_grading_branch", per_student_grading_branch);
			//END2 ;
		}
		PROC;  //New <per_student_grading_branch> with the header
		{
			char grade_sheet_header[256] ;
			sprintf(grade_sheet_header,"@# %s@@", "Please Do Until the {_EXACT_} state echos!!");
			//sprintf(grade_sheet_header,"@# %s, %s@@",student_id, "Please Go Until the tag {_EXACT_} appears at the last line!!");
			SD2("grade_sheet_header", grade_sheet_header);
			// -: @# U02157015, Please Go Until the tag {_EXACT_} appears at the last line!!@@
			new_a_file_with_caption(per_student_grading_branch,grade_sheet_header ) ;
			//END2 ;
		}
	*//////////////////////////////////////////////
	//END2 ;
	//grading file of this project for the graded student_id
/*cjchng
 *
	PROC; const char *CPT_project_grading_all_students_node ;
	{//redirect to the working directory for debugging
		static char project_grading_all_students_node[128] ;
		CPT_project_grading_all_students_node = project_grading_all_students_node ;

		string_path_or_node(CPT_project_grading_all_students_node,"%s%s", CPT_out_project_grading_seg,"all-students/");
		SD2("project_grading_all_store_folder",CPT_project_grading_all_students_node);
		//Value --> Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/all-students/

		if(!checkIfDirExisted(CPT_project_grading_all_students_node) )
		{
			char make_work_forder_cmd[128];
			sprt_sh_cmd(make_work_forder_cmd,"mkdir %s",CPT_project_grading_all_students_node);
			linux_system_cmd_dump( __FILE__, __LINE__, make_work_forder_cmd) ;
			SD2("make_work_forder_cmd", make_work_forder_cmd);
			// -: mkdir 2013-C.jia.wk01.hello_grade/all/
		}
		//END2 ;
	}
cjchng */



	////////////////////////////////////////////////////////////////////////////////////
	//variables for grading
	////////////////////////////////////////////////////////////////////////////////////
	//long int latency_sum=0 ;   		//??
	//long int latency_average=0 ;   	//??
	//int members_valid=0 ;     		//graded student number
	int no_project_state=0;
	int no_binary_state = 0;
	int answer_file_too_large_state = 0 ;
	int no_answer_line_state=0;
	int long_answer_line_state=0;
	int not_exact_state=0;
	int exact_state=0 ;

	enum state {NO_PROJECT=0, NO_BINARY, ANSWER_FILE_TOO_LARGE,
		NO_ANSWER_LINE, LONG_ANSWER_LINE, NOT_EXACT, EXACT};

////////////////////////////////////////////////////////////////////////////////////
ALL_BEGIN;
////////////////////////////////////////////////////////////////////////////////////
	char student_id[128];
	while (read_id(student_id, 128, name_list__rptr) != NULL) {
		//preparing stage
		int step_reached= -1 ;   //begin to grade a student
		SD2("student_id",student_id);

		//END2 ;

		const char *CPT_class_dlt_jy_node ;
		{
			static char class_dlt_jy_node[256];
			CPT_class_dlt_jy_node = class_dlt_jy_node;
			sprintf(CPT_class_dlt_jy_node, "%s%s/%s/", studentS_home_folder, student_id,ws2class);    //2012-1021
			SD2("CPT_class_dlt_jy_node", CPT_class_dlt_jy_node);
			//Value --> /mnt/students-home/U02157001/workspace/C-2013/jia/
			//END2 ;
		}
		/////////////////////////////////////////////////
			PROC; //Naming student echo
			const char * CPT_per_student_graded_leaf ;
			{
				static char s_per_student_graded_leaf[256];
				CPT_per_student_graded_leaf = s_per_student_graded_leaf ;
				sprintf(s_per_student_graded_leaf,"{_%9s_}-echo-%s",student_id, graded_project) ;
				SD2("s_per_student_graded_leaf", s_per_student_graded_leaf);
				CPT_per_student_graded_leaf = s_per_student_graded_leaf ;
				//END2 ;
			}

			PROC; //Naming used in many places
			const char *per_student_grading_branch;
			{
				//char per_student_grading_branch[256];
				static char static_per_student_grading_branch[256];
				per_student_grading_branch = static_per_student_grading_branch ;
				string_path_or_node(per_student_grading_branch,"%s%s", CPT_project_grading_all_students_node, CPT_per_student_graded_leaf) ;
				SD2("per_student_grading_branch", per_student_grading_branch);
				//END2 ;
			}
			PROC;  //New <per_student_grading_branch> with the header
			{
				char grade_sheet_header[256] ;
				sprintf(grade_sheet_header,"@# %s@@", "Please Check Until the {_EXACT_} tag echos at last line!!");
				//sprintf(grade_sheet_header,"@# %s, %s@@",student_id, "Please Go Until the tag {_EXACT_} appears at the last line!!");
				SD2("grade_sheet_header", grade_sheet_header);
				// -: @# U02157015, Please Go Until the tag {_EXACT_} appears at the last line!!@@
				new_a_file_with_caption(per_student_grading_branch,grade_sheet_header ) ;
				//END2 ;
			}
		//////////////////////////////////////////////
/*cjchng
		PROC; //Naming student echo
		const char * CPT_per_student_graded_leaf ;
		{
			static char s_per_student_graded_leaf[256];
			CPT_per_student_graded_leaf = s_per_student_graded_leaf ;
			sprintf(s_per_student_graded_leaf,"{_%9s_}-echo-%s",student_id, graded_project) ;
			SD2("s_per_student_graded_leaf", s_per_student_graded_leaf);
			CPT_per_student_graded_leaf = s_per_student_graded_leaf ;
			//END2 ;
		}

*/


		PROC; //JAVA NOT GOOD
		const char *CPT_per_student_grading_profile_path;
		{
			{
				static char per_student_grading_profile_path[256];
				CPT_per_student_grading_profile_path = per_student_grading_profile_path ;
				sprintf(per_student_grading_profile_path, "%s%s/%s/%s/%s", studentS_home_folder, student_id,"c_profile", chapter_session, CPT_per_student_graded_leaf);   //2012-1021 check grading_holder
				SD2("CPT_per_student_grading_profile_path",CPT_per_student_grading_profile_path);

			}

			const char *CPT_per_student_grading_profile_node;
			{
				static char per_student_grading_profile_node[256];
				CPT_per_student_grading_profile_node = per_student_grading_profile_node ;
				sprintf(per_student_grading_profile_node, "%s%s/%s/%s/", studentS_home_folder, student_id,"c_profile", chapter_session);   //2012-1021 check grading_holder
			}

			//END2 ;
			PROC; //make folder only if not existed
			if(!checkIfDirExisted(CPT_per_student_grading_profile_node))
			{
				char mkdir_per_student_grading_profile_holder[128];
				sprt_sh_cmd(mkdir_per_student_grading_profile_holder, "%s%s", "mkdir -p ", CPT_per_student_grading_profile_node);
				SD2("mkdir_per_student_grading_profile_holder", mkdir_per_student_grading_profile_holder);
				linux_system_cmd_dump( __FILE__, __LINE__, mkdir_per_student_grading_profile_holder);
				SD2("per_student_cur_project_profile_holder", CPT_per_student_grading_profile_node);
			}
		}

		//END2 ;

/** cjchng
		PROC; //Naming used in many places
		const char *per_student_grading_branch;
		{
			//char per_student_grading_branch[256];
			static char static_per_student_grading_branch[256];
			per_student_grading_branch = static_per_student_grading_branch ;
			string_path_or_node(per_student_grading_branch,"%s%s", CPT_project_grading_all_students_node, CPT_per_student_graded_leaf) ;
			SD2("per_student_grading_branch", per_student_grading_branch);
			//END2 ;
		}
		PROC;  //New <per_student_grading_branch> with the header
		{
			char grade_sheet_header[256] ;
			sprintf(grade_sheet_header,"@# %s@@", "Please Do Until the {_EXACT_} state echos!!");
			//sprintf(grade_sheet_header,"@# %s, %s@@",student_id, "Please Go Until the tag {_EXACT_} appears at the last line!!");
			SD2("grade_sheet_header", grade_sheet_header);
			// -: @# U02157015, Please Go Until the tag {_EXACT_} appears at the last line!!@@
			new_a_file_with_caption(per_student_grading_branch,grade_sheet_header ) ;
			//END2 ;
		}

*/

		PROC;//Constant Message line presented  at the graded sheet of the current [project] && [student]
		{
			char msg_of_this_graded_id_to_grading_file[256] ;
			sprintf(msg_of_this_graded_id_to_grading_file, "ID:[%9s], Class:[%s], Session:[%s], Project:[%s]", student_id, kcmc, chapter_session, graded_project); //,
			head_of_grading_path(per_student_grading_branch, msg_of_this_graded_id_to_grading_file);
			SD2("per_student_grading_branch",per_student_grading_branch);
			SD2("msg_of_this_graded_id_to_grading_file",msg_of_this_graded_id_to_grading_file);
			//Value --> ID:[U02157001], Class:[C-2013.jia], Session:[wk06-c5], Project:[fig05_12]
			//END2 ;
		}

		PROC;
		const char *CPT_cur_student_project_node;
		{
			static char cur_student_project_node[256];
			CPT_cur_student_project_node = cur_student_project_node ;

			sprintf(CPT_cur_student_project_node, "%s%s/%s",CPT_class_dlt_jy_node, chapter_session,	graded_project);
			SD2("studentS_class_node",CPT_class_dlt_jy_node);
			SD2("cur_student_project_node",CPT_cur_student_project_node);
			//Value --> /mnt/students-home/U02157001/workspace/C-2013/jia/wk06-c5/fig05_12
			//END2 ;
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	End of student records preparation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		PROC;//[CHECK IF STUDENT ANSWER]{1} No Project found
		{
			//STAGE #1 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			DIR *dir ;
			if ((dir = opendir(CPT_cur_student_project_node)) == NULL) { //no project holder presents
				SD2("(Classified Stage#1, can not open folder) CPT_cur_student_project_node", CPT_cur_student_project_node );
				char no_found_msg[128];
				{//state student_id to absent path for ta
					append_left_string_2file(student_id, CPT_absent_branch);     //1218-2012
					char reporting_absent_for_this_id[256];{
						sprintf(reporting_absent_for_this_id, "                                          ID:[%9s],{STATE_0}: No project folder", student_id) ;
					}
					SD2("reporting_absent_for_this_id",reporting_absent_for_this_id);
					// -:                                          ID:[U02157099], {STATE_0}: No project folder
					ECHO_NO_PROJECT_STATE(reporting_absent_for_this_id) ;
					//  -> append_string_2file(reporting_absent_for_this_id, CPT_running_record_summary);
					//  -> append_string_2file((OUT_STRING), per_student_grading_branch)
					no_project_state++ ;
					step_reached = NO_PROJECT;
				}

				{ //append this student grading path to class grading path
					cat_left_file_append_2_right_file(per_student_grading_branch,CPT_all_ID_grading_sheet_branch );

				}
				//EXIT_STATE_0() ;
				//junk_copy_student_grade_to_ta_and_to_his_profile(CPT_runners_grade_all_node, CPT_per_student_grading_profile_node,
				//		CPT_per_student_graded_leaf, per_student_grading_branch )	;
				copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;
				SD2("per_student_grading_branch", per_student_grading_branch) ;
				SD2("project_grading_injection_path",CPT_all_ID_grading_sheet_branch ) ;
				continue; //No project, Absent Next Steps
			}
		}

		PROC;
		const char * CPT_per_student_exe_path ;
		{  //the project folder is found, stepping to stage #2
			//STAGE #2 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//{

			static char s_per_student_exe_path[512];   //the target from student, and will be graded
			CPT_per_student_exe_path = s_per_student_exe_path ;
			{
				strcpy(s_per_student_exe_path, "");
				strcat(s_per_student_exe_path, CPT_cur_student_project_node);
				strcat(s_per_student_exe_path, RUN_OR_DEBUG);
				strcat(s_per_student_exe_path, graded_project); //student's execution file in debug|run folder
			}
			SD2("s_per_student_exe_path", s_per_student_exe_path) ;
			//Value --> /mnt/students-home/U02157001/workspace/C-2013/jia/wk06-c5/fig05_12/Debug/fig05_12
			//END2 ;
		}

		PROC;//[CHECK IF STUDENT ANSWER]{2} no binary, otherwise, get her/his deployed date
		gnu_time_t cur_student_last_access_time ;
		char date_cur_binary[128];
		{//if state is zero
			int fileOK=1 ;
			cur_student_last_access_time=get_modify_time(CPT_per_student_exe_path, student_exe_stat_buf,date_cur_binary, &fileOK) ;
			SD2("date_cur_binary",date_cur_binary) ;
			//Value --> Tue_Oct_29_15:46:07_2013

			if(fileOK ==0) { //no binary found
				append_left_string_2file(student_id, CPT_no_exe_branch);      //1218-2012

				/////////////////////////////////
				char reporting_no_exe[128];{
					//Format output message for this account
					sprintf(reporting_no_exe, "                                          ID:[%9s],{STATE_1}: No execution file", student_id) ;
				}

				//append_string_2file(no_exe_2record, CPT_running_record_summary);
				ECHO_NO_BINARY_STATE(reporting_no_exe) ;
				no_binary_state++ ;
				step_reached =  NO_BINARY ;
				append_right_path_to_left_path(CPT_all_ID_grading_sheet_branch,  per_student_grading_branch);
				SD2("project_grading_injection_path", CPT_all_ID_grading_sheet_branch );
				//EXIT_STATE_1() ;
				//junk_copy_student_grade_to_ta_and_to_his_profile(CPT_runners_grade_all_node, CPT_per_student_grading_profile_node,
				//		CPT_per_student_graded_leaf, per_student_grading_branch )	;
				copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;
				continue ;    //No Binary, switch to another student
			}  //else is next PROC;
		}

		//END2 ;
		PROC;   //[CHECK IF STUDENT ANSWER]{9} using too long running binary
		long int latency;  //Student finished time
		char cur_ID_answer_leaf[]= "asset/a_student-answer_leaf" ;	//his answer
		char Student_Latency_Msg[256];		//A brief line
		{
			{
				// Stage #3, found the binary //////////////////////////////////
				printf("cur_student_last_access_time at %ld\n",cur_student_last_access_time.tv_sec );
				printf("ta_last_access_time at   %ld\n",ta_last_access_time.tv_sec );

				printf("ta diff = %ld\n", latency = cur_student_last_access_time.tv_sec-ta_last_access_time.tv_sec);
				{
					char latency2ascii[12] ;
					sprintf(latency2ascii, "%ld", latency);
					SD2("latency2ascii", latency2ascii);
				}
			}
			//END2 ;
			//////////////////////////////
			PROC;//accident execution mode violation prevention
			{
				char chmod_cmd[256];
				sprt_sh_cmd(chmod_cmd, "chmod +x %s", CPT_per_student_exe_path);
				linux_system_cmd_dump( __FILE__, __LINE__, chmod_cmd);
			}
			////////////////////////////////////////////
			PROC; char bin_symbol_ln[256];  //the symbol link used to execute student binary
			//remove this name after grading procedure
			{
				linux_system_cmd_dump(__FILE__, __LINE__, "mkdir -p asset/bin/ln");

				sprintf(bin_symbol_ln, "asset/bin/ln/%s.%s.%s", student_id, chapter_session, graded_project);
				SD2("bin_symbol_ln",bin_symbol_ln);
				//Value --> asset/bin/ln/U02157001.wk06-c5.fig05_12
				/*
				if(fopen(bin_symbol_ln, "r") == NULL)    	//prevent can not create symbol link error,
					//if crash without delete the previous symbol link
					//the system will echo error if the symbol linked is symbolic link once more
				{
					char sym_ln_cmd[256]; //
					//compose a linux shell command as the symbol link command point to the graded project's execution
					sprt_sh_cmd(sym_ln_cmd, "ln -s %s  %s", CPT_per_student_exe_path, bin_symbol_ln);
					SD2("(symbol link cmd is)sym_ln_cmd ", sym_ln_cmd);
					/// ln -s /mnt/students-home/U02157024/workspace/C-2013/jia/wk06-c5/fig05_03/Debug/fig05_03  src/bin/ln/U02157024.wk06-c5.fig05_03
					linux_system_cmd_dump(__FILE__, __LINE__, sym_ln_cmd); //generated the symbol link to invoke student binary
				}*/
				//update_sln(char *linked, char *sl)
				update_sln(CPT_per_student_exe_path, bin_symbol_ln) ;

			}
			//END2 ;

			/*
					./myProgram &
					sleep 1
					kill $! 2>/dev/null && echo "myProgram didn't finish"
			 */
			//char grader_nfs_node[]="~/";


			PROC; //I prefer keeping this module
			{

				PROC; //add header to per_student_grading_branch
				{
					char fmt[128] ;
					sprintf(fmt, "%s%s", cons_fmt_node(), "plus_line.dlt");
					cat_left_file_append_2_right_file(fmt,per_student_grading_branch);
					sprintf(fmt, "%s%s", cons_fmt_node(), "space_line.dlt");
					cat_left_file_append_2_right_file(fmt,per_student_grading_branch);
					SD2("fmt",fmt) ;
					//Value --> asset/fmt/space_line.dlt
					SD2("per_student_grading_branch",per_student_grading_branch);
					//Value --> Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/all-students/{_U02157001_}-echo-fig05_12
					//END2;
				}
				PROC; //copy_TAs_solution_2_per_student_grading_branch_command
				if(0) //cjchng
				{
					append_left_string_2file(" ", per_student_grading_branch);
					append_left_string_2file("[cj::BEGIN SOLUTION COPY FROM FROM TA]", per_student_grading_branch);

					char copy_TAs_solution_2_per_student_grading_branch_command[512];
					sprt_sh_cmd(copy_TAs_solution_2_per_student_grading_branch_command, "cat %s >>%s", CPT_ta_solutions_path, per_student_grading_branch);
					linux_system_cmd_dump(__FILE__, __LINE__, copy_TAs_solution_2_per_student_grading_branch_command);
					SD2("copy_TAs_solution_2_student_grade_file_command", copy_TAs_solution_2_per_student_grading_branch_command);
					//Value --> cat ./asset/pseudoFTP/grading/ta-gives-ans/C-2013.jia.wk06-c5.fig05_12-1220_e.ans  \
					//          >>Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/all-students/{_U02157001_}-echo-fig05_12
					//END2 ;
				}
			}
			PROC; //begin present student echo, (1)show annotated tag to per_student_grading_branch
			{
				//echo_dlt_string("[cj::---------------------------]", per_student_grading_branch);
				if(0) //not need next statements
				{
					char buf[128];
					sprintf(buf,"@[ %s Echoing...]",student_id );
					append_left_string_2file("@[BEGIN YOUR ANSWER!]", per_student_grading_branch);
					append_left_string_2file(buf, per_student_grading_branch);
				}
				//END2;

				/*if(OBSOLETE) //Do you want to delete next segment, no function
				{ //create the folder for a new chapter_session
					//I don't know why we do this action in this place
					char per_project_grading_record_folder[256] ;
					char mkdir_cmd[256];
					sprintf(per_project_grading_record_folder, "%s%s/%s/",grading_node,kcmc,chapter_session);
					sprt_sh_cmd(mkdir_cmd, "mkdir -p %s", per_project_grading_record_folder );
					exe_system_cmd_dump(__FILE__, __LINE__, mkdir_cmd);
					{{
						SD2("mkdir_cmd", mkdir_cmd ) ;
						END2 ;
						//-: mkdir -p /home/grda/pseudoFTP/grading/2013-C.jia/wk01-c2/
					}}
				}*/

				//END2 ;
				/////////////////////////////////////////////////////////////////////
				PROC; //comment for the applied algorithm
				{/*
				//template to limit execution time
				# ./myProgram &
				# sleep 1
				# kill $! 2>/dev/null && echo "myProgram didn't finish"
				 */
				}


				{//constructing part of latency line for grading student
					pack_latency_common_msg(Student_Latency_Msg,latency,date_cur_binary,student_id);
					SD2("Student_Latency_Msg",Student_Latency_Msg);
					//Value --> +0001283 Index,[Tue_Oct_29_15:46:07_2013] ID:[U02157001]
					//END2 ;
				}

				PROC; //testing Running student binary
				{ //Implement the shell script to execute student binary in background limited by time quantum limit specified
					char student_grade_script_branch[256] ;
					{
						char tmpDstudentMscript[] =  "asset/tmp.students-script/" ;
						if( !checkIfDirExisted(tmpDstudentMscript))
						{
							char mk_tmpDstudentMscript_seg[128] ;
							sprintf(mk_tmpDstudentMscript_seg,"mkdir -p %s", tmpDstudentMscript) ;
							linux_system_cmd_dump(__FILE__, __LINE__, mk_tmpDstudentMscript_seg) ;
						}

						sprintf(student_grade_script_branch, "%s%s%s-%s", tmpDstudentMscript,"grade-", student_id, graded_project);    //asset
						SD2("student_grade_script_branch", student_grade_script_branch);
						//Value --> asset/tmp.students-script/grade-U02157001

						//END2 ;
						FILE *afp = fopen(student_grade_script_branch, "w") ;
						if(afp == NULL){
							SD2("Open file for write error!!", student_grade_script_branch );
							fatal_end(__LINE__, __FILE__);
						}
						///////////////////////////////////////////////////
						char backgound_sh[1024] ;
						{ //student grading shell command
							char cur_id__project_bin_sh_cmd[512] ; //this command read arguments from file and out result to
							sprintf(cur_id__project_bin_sh_cmd, "%s%s%s%s%s%s%s",
									bin_symbol_ln, " <",
									CPT_injections_y_source_code_path, " >",
									cur_ID_answer_leaf, " 2>",
									cur_ID_answer_leaf);
							SD2("(STUDENT-CMD:)cur_id__project_sh_cmd", cur_id__project_bin_sh_cmd);
							//Value --> asset/bin/ln/U02157001.wk06-c5.fig05_12 <Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/injection_y_source_code >student-answer_leaf 2>student-answer_leaf
							//this command will run in background
							sprintf(backgound_sh, "%s &", cur_id__project_bin_sh_cmd) ;   //./myProgram &
						}
						fprintf(afp, "%s\n", backgound_sh) ;
						PROC; //foreground job
						char usleep_quantum_cmd[129] ;
						{
							const unsigned int QUANTUM=100000;
							sprintf(usleep_quantum_cmd, "asset/bin/usleep %u\n", QUANTUM);
							SD2("usleep_quantum", usleep_quantum_cmd );     //configurable is better
						}
						fprintf(afp, "%s", usleep_quantum_cmd);
						fprintf(afp, "kill $! && echo \"program use CPU > 100ms!!\" 2>/dev/null") ;     // kill $! 2>/dev/null && echo "myProgram didn't finish"

						fclose(afp);
					}
					//END2;
					PROC;
					{ //change student_grade_script_branch mode to executable
						char aBuf[128] ;
						sprintf(aBuf,"chmod +x %s", student_grade_script_branch);
						{{
							SD2("Change Mode Command", aBuf);
							// -:chmod +x /home/grda/grade-U02157015
						}}
						linux_system_cmd_dump(__FILE__, __LINE__, aBuf) ;
					}

					PROC;
					{ //execute student project
						char evaluateing_cur_student_sh[128] ;
						sprintf(evaluateing_cur_student_sh, ". %s", student_grade_script_branch) ;

						//linux_system_cmd_dump(__FILE__, __LINE__, evaluateing_cur_student_sh);
						system(evaluateing_cur_student_sh) ;  //cjchng

						SD2("(Grading...) evaluateing_cur_student_sh", evaluateing_cur_student_sh);
						//Value --> . asset/tmp.students-script/grade-U02157001-fig05_12
					}


					PROC; //rm_student_grade_path
					if(0)//cjchng
					{
						char rm_student_grade_branch[132];
						sprintf(rm_student_grade_branch,"rm %s", student_grade_script_branch);
						SD2("rm_student_grade_branch", rm_student_grade_branch);
						linux_system_cmd_dump(__FILE__, __LINE__, rm_student_grade_branch);
					}

					PROC; //remove <bin_symbol_ln>
					if(0)//cjchng
					{
						char rm_symbol_link[128];
						sprintf(rm_symbol_link, "rm %s", bin_symbol_ln) ;
						SD2("rm_symbol_link", rm_symbol_link);
						linux_system_cmd_dump(__FILE__, __LINE__, rm_symbol_link);
					}
					/*
					if(OVER_RUN) {
						over_run++ ;
						over_run_continue() ;
						continue ;
					}
					*/

				}  //end of procedure testing run student binary
			}
		}
////////////////////////////////////////////////////////
		PROC; //[CHECK IF STUDENT ANSWER]{3} size is too large
		{
			PROC; // Processing if answer line is too long
			{
				int size_of_answer_leaf;
				{//1119-2012
					char  size[12] ;
					sprintf(size, "%d", size_of_answer_leaf = size_of_file(cur_ID_answer_leaf));
					SD2("size_of_answer_leaf",size);
				}

				if( size_of_answer_leaf > 1000)   //why 1000?
				{//answer file too large
					char del_short_grading_name[128] ;
					sprt_sh_cmd(del_short_grading_name,"rm %s", cur_ID_answer_leaf );
					linux_system_cmd_dump(__FILE__, __LINE__, del_short_grading_name) ;

					char size_too_large_msg[128];
					{
						sprintf(size_too_large_msg, ", {STATE_2}:answer size :%d", size_of_answer_leaf );
					}
					strcat(Student_Latency_Msg, size_too_large_msg);
					//append_string_2file(student_latency_msg, per_student_grading_branch);
					//append_string_2file(student_latency_msg, CPT_running_record_summary);
					ECHO_ANSWER_FILE_TOO_LARGE_STATE(Student_Latency_Msg) ;
					answer_file_too_large_state++;
					step_reached =  ANSWER_FILE_TOO_LARGE;   //1128
					append_right_path_to_left_path(CPT_all_ID_grading_sheet_branch,  per_student_grading_branch);
					//EXIT_STATE_2() ;
					//junk_copy_student_grade_to_ta_and_to_his_profile(CPT_runners_grade_all_node, CPT_per_student_grading_profile_node,
					//		CPT_per_student_graded_leaf, per_student_grading_branch )	;
					copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;
					continue ; //ANSWER_FILE_TOO_LARGE, Switch student
					///////////////////////////////////////////////////////////////////////////////////////
				}
				/*else {
				sh_cat_left_file_append_2_right_file(cur_ID_answer_leaf, per_student_grading_branch);
				append_left_string_2file("@[END YOUR ANSWER]", per_student_grading_branch);
				}*/
			}
		} //End of Answer file too large

		PROC; // Answer file save since within a suitable size
		append_student_answer2student_grading_sheet( per_student_grading_branch, cur_ID_answer_leaf);   //1229

		PROC; //[CHECK IF STUDENT ANSWER]{4} answer line is too long
		char  student_ans_last_line[1024];
		int line_no=0 ;
		{//process the answer output from student project
			FILE *fp_1110;

			int break_by_while_loop = 0 ;
			fp_1110=fopen(cur_ID_answer_leaf, "r") ;
			while (fgets(student_ans_last_line, sizeof(student_ans_last_line), fp_1110) != NULL){
				if( strlen(student_ans_last_line) > 128){    //why 128
					char answer_line_greater_than_128[128] ;
					{
						//sprintf(answer_line_greater_than_128, ", STATE_:answer_line_greater_than_128" );
						sprintf(answer_line_greater_than_128, ", {STATE_3}:%s", student_ans_last_line );
					}
					strcat(Student_Latency_Msg, answer_line_greater_than_128);

					ECHO_LONG_ANSWER_LINE_STATE(Student_Latency_Msg) ;
					long_answer_line_state++ ;
					step_reached = LONG_ANSWER_LINE ;
					//append_right_path_to_left_path(project_grading_path,  per_student_grading_branch);
					break_by_while_loop = 1 ;

					// will EXIT_STATE_3() ;
					//junk_copy_student_grade_to_ta_and_to_his_profile(CPT_runners_grade_all_node, CPT_per_student_grading_profile_node,
					//		CPT_per_student_graded_leaf, per_student_grading_branch )	;
					copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;
					continue ; //this continue is [while loop] and not next student id [for loop]
				}
				line_no++ ;
			}
			if(break_by_while_loop) {
				append_right_path_to_left_path(CPT_all_ID_grading_sheet_branch,  per_student_grading_branch);
				continue ;  //Answer too long, Switch student
				//////STAGE #3 END //////////////////////////////////////////////////////////////////////////////
			}
		}

		PROC;//[CHECK IF STUDENT ANSWER]{5} has no answer line
		{
			if(line_no ==0) {
				char no_answer_line[128] ;
				{
					sprintf(no_answer_line, ",{STATE_4}:Not Any Answer Data");
					strcat(Student_Latency_Msg, no_answer_line);

					ECHO_NO_ANSWER_LINE_STATE(Student_Latency_Msg) ;
					no_answer_line_state++ ;
					step_reached = NO_ANSWER_LINE ;
					append_right_path_to_left_path(CPT_all_ID_grading_sheet_branch,  per_student_grading_branch);
					//EXIT_STATE_4() ;
					//junk_copy_student_grade_to_ta_and_to_his_profile(CPT_runners_grade_all_node, CPT_per_student_grading_profile_node,
					//		CPT_per_student_graded_leaf, per_student_grading_branch )	;
					copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;
					continue ;   //No answer line, switch student
					/////Break the No answer state(#4)///////////////////////////////////
				}
			}
		}
		//end of STATE_4

		//printf("\n\n%s\n\n\n", ctime(&stat_buf.st_mtim));
		//&stat_buf.st_mtim

		PROC;//[CHECK IF STUDENT ANSWER]{6} not exact
		{ //Stage 5//2012-11-10
			//SD2("student_ans_last_line",student_ans_last_line);
			//SD2("ta_ans_last_line",ta_ans_last_line);

			if(!answer_is_good(ta_ans_last_line, student_ans_last_line)){

				//sprintf(student_latency_msg, "%+08ld Index,{%s} ID:[%s], ?????:%s[cj]",
				//		latency, student_date, student_id, student_ans_last_line );
				PROC; //append state tag and show answer(last line only)
				{
					char brief_grade[128];
					{
						sprintf(brief_grade,",{STATE_5}:%s",student_ans_last_line);
					}
					strcat(Student_Latency_Msg, brief_grade) ;
				SD2("student_latency_msg",Student_Latency_Msg);
				}
				ECHO_NOT_EXACT_STATE(Student_Latency_Msg) ;
				not_exact_state++ ;
				step_reached = NOT_EXACT ;
				append_right_path_to_left_path(CPT_all_ID_grading_sheet_branch,  per_student_grading_branch);
				copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;

				continue ;   //end of State 5, Answer is not exact, Switch Account
			}
		}
//END2;
		PROC; //[CHECK IF STUDENT ANSWER]{7} EXACT
		{//EXACT STATE
			step_reached = EXACT ;
			{
				PROC;//
				{
					char exact_brief_grade[128];
					{
						sprintf(exact_brief_grade,",{_EXACT_}:%s", student_ans_last_line);
						sD2(__LINE__,__FILE__,"exact_brief_grade",exact_brief_grade);
					}
					strcat(Student_Latency_Msg, exact_brief_grade) ;
					sD2(__LINE__,__FILE__,"student_latency_msg",Student_Latency_Msg);
				}
				//latency_sum += latency ;
				//members_valid++ ;
				if(1)
				{//beauty format
					append_left_string_2file("////Line added by Gradder",	per_student_grading_branch);
				}

				{//copy student's code to instructor for visually answering check
					char per_student_source_code__path[256];
					char tmp[128];
					sprintf(tmp, "### [%s] %s", student_id," CODE BEGIN HERE");
					append_left_string_2file(tmp,	CPT_all_ID_grading_sheet_branch);
					string_path_or_node(per_student_source_code__path,
							"%s%s/%s/src/%s.c",//.c =>c .cpp =>c++
							CPT_class_dlt_jy_node,
							chapter_session,
							graded_project,
							graded_project);
					cat_left_file_append_2_right_file(cur_ID_answer_leaf, CPT_all_ID_grading_sheet_branch );
				}

				PROC;
				{
					copy_ta_src_to_gradind_file(fAppendTaSourceToStudentProfile, per_student_grading_branch, CPT_ta_c_source_path );
				}
			}

			copy_right_path_to_left_path(CPT_per_student_grading_profile_path, per_student_grading_branch) ;
			ECHO_EXACT_STATE(Student_Latency_Msg) ;
			exact_state++ ;
			append_right_path_to_left_path(CPT_all_ID_grading_sheet_branch,  per_student_grading_branch);
			continue ;//EXACT; Switch student
		}
	}   //all students were processed!!
//END2 ;
	PROC; //the final stage begin here
	{
		char wrap_up[7][256];
		printf("%40s:%d\n", "no_project_state", no_project_state) ;
		printf("%40s:%d\n", "no_binary_state" , no_binary_state) ;
		printf("%40s:%d\n", "answer_file_too_large_state" , answer_file_too_large_state) ;
		printf("%40s:%d\n", "no_answer_line_state", no_answer_line_state) ;
		printf("%40s:%d\n", "long_answer_line_state",long_answer_line_state) ;
		printf("%40s:%d\n", "not_exact_state", not_exact_state) ;
		printf("%40s:%d\n", "exact_state", exact_state) ;

		sprintf(wrap_up[0], "%30s:%d", "STATE: no_project_state", no_project_state) ;
		sprintf(wrap_up[1], "%30s:%d", "STATE: no_binary_state" , no_binary_state) ;
		sprintf(wrap_up[2], "%30s:%d", "STATE: answer_file_too_large_state" , answer_file_too_large_state) ;
		sprintf(wrap_up[3], "%30s:%d", "STATE: long_answer_line_state",long_answer_line_state) ;
		sprintf(wrap_up[4], "%30s:%d", "STATE: no_answer_line_state", no_answer_line_state) ;
		sprintf(wrap_up[5], "%30s:%d", "STATE: not_exact_state", not_exact_state) ;
		sprintf(wrap_up[6], "%30s:%d", "EXACT: exact_state", exact_state) ;

		append_left_string_2file("", CPT_running_recordS_summary) ;
		int ai; for(ai=0; ai<7; ai++){
			append_left_string_2file(wrap_up[ai], CPT_running_recordS_summary) ;
		}
		append_left_string_2file("", CPT_running_recordS_summary) ;
		sprintf(wrap_up[6], "%30s:%d", "graded total",no_project_state + no_binary_state + no_answer_line_state \
				+long_answer_line_state + not_exact_state + exact_state  ) ;
		append_left_string_2file(wrap_up[6], CPT_running_recordS_summary) ;
	}
	SD2("CPT_running_record_summary", CPT_running_recordS_summary);

	/*0104cat_left_file_append_2_right_file(CPT_running_recordS_summary, DOT_SUMMARY);

	SD2("CPT_BRIEF_OF_THIS_PROJECT", DOT_SUMMARY);*/
	//Value --> ./grading/C-2013.jia/wk06-c5/fig05_12.summary
	//END2;
	SD2("CPT_running_record_summary", CPT_running_recordS_summary);

	{//Delete last symbol link for last graded project in order to update the the current session
		//Since the Get/running-records-summary leaf is repeatly used
		char reLinkedGetDotCPT_running_record_summary[256];  //cjchng
		sprintf(reLinkedGetDotCPT_running_record_summary, "%s/%s", current_worked_dir, CPT_running_recordS_summary);
		SD2("reLinkedGetDotCPT_running_record_summary", reLinkedGetDotCPT_running_record_summary);
		update_sln(reLinkedGetDotCPT_running_record_summary, "Get/running-records-summary");
		SD2("reLinkedGetDotCPT_running_record_summary", reLinkedGetDotCPT_running_record_summary);
		//Value --> /home/user/workspace/grd-jc_2013-12-02/Out/assm.wk06-c5.C-2013.jia/fig05_12@1220_e/running-records-summary
	}
	update_sln(CPT_out_project_grading_seg, "Grading...") ;


	//END2 ;
	//cat_left_file_append_2_right_file
	/* 2012/12/10
	///////////////////////////////////////////////////////////////////////////////
		{ //copy record_running that containing answer STATE summary
			  //source: CPT_running_record_summary
			  //destination:to_ta_per_project_summary_path
				char to_taing_per_project_summary_path[256] ;
				{ //
					sprintf(to_taing_per_project_summary_path, "%s%s",ta_grade_folder, CPT_running_record_summary) ;
				}

				char copy_project_graded_summary_to_taing_project_cmd[256];
				{//write latency_msg to the specifc latency file
					sprintf(copy_project_graded_summary_to_taing_project_cmd,"cp %s %s",
							summary_of_this_class,
							to_taing_per_project_summary_path);
				}
				system(copy_project_graded_summary_to_taing_project_cmd);
		}
	///////////////////////////////////////////////////////////////////////////
	 */

	//Append ta's C Source Code to TA's Solution Files
	{   //the problem issues should be redesigned
		/*
		append_left_string_2file("HERE IS THE OUESTION", ans_join_to_TA_solutions_set_path);
		{
			char problem_node[256];
			sprintf(problem_node, "%sstat/problem", taf_only_ta_project_folder);
			SD2("problem_node",problem_node);
			sh_cat_left_file_append_2_right_file(problem_node, ans_join_to_TA_solutions_set_path);
		}
		 */
	}



	////////////////////////////////////////////////////////////////////////////
	//sort latency
	///////////////////////////////////////////////////////////////////////////

	{//_grading_holder

		//FILE *fp_1107;
		/*char latency_sort_path[256] ;
		//sprintf(latency_sort_path, "%slatency.sort",solution_account_folder);
		sprintf(latency_sort_path, "%s%s/%s/%s.latency",grading_node,kcmc,chapter_session,graded_project);
		printf("latency_sort_path is [%s]\n",latency_sort_path);
		open_file_for_write(latency_sort_path);*/

		///////////////////////////////////////////////////////////////
		char latency_sort[256]  ;
		sprintf(latency_sort, "%s%s",CPT_out_project_grading_seg, "latency.sort" );
		char sort_cmd[128] ;
		sprt_sh_cmd(sort_cmd, "cat %s | sort -n > %s", latency_no_sort, latency_sort);
		printf("sort_cmd is [%s]\n", sort_cmd) ;
		linux_system_cmd_dump(__FILE__, __LINE__, sort_cmd);

		//put_line_to_lfile_head(char *file, char *line) ;
		put_line_to_lfile_head(latency_sort, "") ;
		put_line_to_lfile_head(latency_sort, copy_right_logo) ;


		/////////////////////////////////////////////////////////////
		//0104 new_a_file_with_caption(latency_sort_path, copy_right_logo);
		/////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////
		{   //cjchng 1101
			char exact_sort[256] ;
			sprintf(exact_sort,"%s%s", CPT_out_project_grading_seg, "exact.sort");
			char sort_cmd[128] ;
			sprt_sh_cmd(sort_cmd, "cat %s | sort -n > %s", exact_no_sort, exact_sort);
			SD2("sort_cmd is", sort_cmd) ;
			linux_system_cmd_dump(__FILE__, __LINE__, sort_cmd);
			/////////////////////////////////////////////////////////////
			//new_file_with_hello(latency_sort_path, copy_right_logo);
			/*
			{
					char cmdBuf[128];
					sprintf(cmdBuf, "ln -s %s/%s %s", cwd,exact_sort, "job/z_exact.sort.GET");
					SD2("exact_sort", exact_sort);
					SD2("cmdBuf",cmdBuf);
					system(cmdBuf);
			}
			 */
			{
				char linked[128];
				sprintf(linked, "%s/%s", current_worked_dir, exact_sort);
				SD2("linkde", linked);
				update_sln(linked, "Get/exact.sort");
			}
			{
				char linked[128];
				sprintf(linked, "%s/%s", current_worked_dir, CPT_out_ta_solution_branch);
				printf("strlen(linked) =[%d]", strlen(linked));
				SD2("linked", linked);
				update_sln(linked, "Get/ta-solution_leaf");

			}

			if(1) //dump sorted exact
			{
				char dumpSortedExacts[256];

				printf("\n??????????????????????????????????????????????????");
				sprt_sh_cmd(dumpSortedExacts, "cat %s", exact_sort) ;
				SD2("dumpSortedExacts",dumpSortedExacts);

				SD2("+++Exact List:","...");
				linux_system_cmd_dump(__FILE__, __LINE__, dumpSortedExacts) ;
				SD2("+++Exact List:","end.");
			}
		}


		put_line_to_lfile_head("Get/exact.sort", "") ;
		put_line_to_lfile_head("Get/exact.sort", copy_right_logo) ;
		//END2 ;
		/*
		{
			FILE *fp_sort ;
			if( (fp_sort=fopen("Get/exact.sort","r")) == NULL)
			{
				SD2("ERROR::", "No file to read") ;
				END;
			} else
			{
				char line[128], buf[128];
				char *apt ;
				int sequence = 1 ;

				SD2("CPT_BRIEF_OF_THIS_PROJECT",DOT_SUMMARY);
				append_left_string_2file(" " , DOT_SUMMARY) ;
				append_left_string_2file(" " , DOT_SUMMARY) ;
				append_left_string_2file("<<<<Here is the exact list>>>> " , DOT_SUMMARY) ;
				append_left_string_2file(" " , DOT_SUMMARY) ;

				while (fgets(line, sizeof(line), fp_sort) != NULL){
					apt=strstr(line, "[") ;
					//printf("%s", apt) ;
					sprintf(buf, "{%02d} ", sequence ++);
					strcat(buf, apt);
					printf("%s", buf) ;
					SD2("buf",buf);
					append_left_string_2file(buf, DOT_SUMMARY) ;
				}
				SD2("buf",buf);
				END ;
			}
		}
		*/////////////////////////////////////////////////////////////
		//SD2("latency_sort_path",latency_sort_path) ;
		//append_left_string_2file(CPT_ta_latency_msg,	latency_sort_path);

		//cat_left_file_append_2_right_file("~/ftp/grading/fmt/minus_line.dlt",	latency_sort_path);
		//cat_left_file_append_2_right_file(latency_sort, latency_sort_path);
		//cat_left_file_append_2_right_file("~/ftp/grading/fmt/minus_line.dlt",	latency_sort_path);
		//append_left_string_2file(CPT_ta_latency_msg,	latency_sort_path);
		//cat_left_file_append_2_right_file("~/ftp/grading/fmt/plus_line.dlt",	latency_sort_path);
	}

	/*///////////////////////////////////////////////////////////////////////////////////////
	char copy_project_stat_to_homework_grade_cmd[256];
	sprt_sh_cmd(copy_project_stat_to_homework_grade_cmd, "cp -r %sstat/ %s",CPT_ta_project_folder, CPT_out_project_grading_seg) ;
	printf(copy_project_stat_to_homework_grade_cmd," =%s",copy_project_stat_to_homework_grade_cmd);
	linux_system_cmd_dump(__FILE__, __LINE__, copy_project_stat_to_homework_grade_cmd);
	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	char copy_homework_grade_to_project_cmd[256];
	sprt_sh_cmd(copy_homework_grade_to_project_cmd, "cp -r %s %s",CPT_out_project_grading_seg, CPT_runner_node) ;
	printf(copy_homework_grade_to_project_cmd," =%s",copy_homework_grade_to_project_cmd);
	linux_system_cmd_dump(__FILE__, __LINE__, copy_homework_grade_to_project_cmd);
	/////////////////////////////////////////////////////////////////////////////////////////*/

	free(current_worked_dir);
	printf("\n\nEXIT OK\n");
	return 0;
}


//extern char pseudoFtpNode[];
//extern char fmt_node[];
//extern char per_project_graded_answer_node[] ;    //set in this function
//
//extern char ta_solution_node[] ;
//extern char grading_node[] ;

void consGrdNodes(char *taAccount)
{
	return ;
	//char asset_pseudoFtp_Node [] = "./asset/pseudoFTP/" ;	///where to hold the graded result for grading student's semester academic record
													/// a project graded will distribute its results to the associated grading node as well as to
													/// assessment node

	char per_project_graded_answer_node[256] ;
	char asset_pseudoFtp_Node [] = "./" ;
	char grading_bridge[] 	= "grading/";    	    ////2012-1021 check grading_holder
	SD2("taAccount",taAccount);

	//char per_project_graded_answer_bridge[]= "grading/ANS/";
	char per_project_graded_answer_bridge[]= "";
	char ta_solution_bridege[128];
	sprintf(ta_solution_bridege, "%s%s-gives-ans/", grading_bridge, taAccount);

	SD2("ta_solution_bridege",ta_solution_bridege);
	//strcpy(fmt_node, 						asset_pseudoFtp_Node);
	strcpy(per_project_graded_answer_node, 	asset_pseudoFtp_Node );
	strcpy(ta_solution_node, 				asset_pseudoFtp_Node );

	{//construct grading node
		strcpy(grading_node, asset_pseudoFtp_Node );
		strcat(grading_node, grading_bridge );
		SD2("grading_node",grading_node);
	}

	{
	strcat(per_project_graded_answer_node, per_project_graded_answer_bridge );
	SD2("per_project_graded_answer_node",per_project_graded_answer_node);
	strcat(ta_solution_node, ta_solution_bridege );
	SD2("ta_solution_node",ta_solution_node);
	//END2 ;
	}
}

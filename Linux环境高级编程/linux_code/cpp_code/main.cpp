#include "../include/bpt.h"
#include "../include/TextTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <iostream>
#include <fstream>

using namespace bpt;
using namespace std;

const char *errorMessage = "> your input is invalid,print \".help\" for more infomation!\n";
const char *nextLineHeader ="> ";
const char *exitMessage = "> see you!\n";
const char *dbFileName = "./data/bpt_db.bin";

clock_t startTime,finishTime;//利用clock函数计算命令所需要的时间

void printHelpMess();
void selectCommand();
int insertRecord(bplus_tree *treePtr,int *key, value_t *values);
int deleteRecord(bplus_tree *treePtr,int *index);
int searchRecord(bplus_tree *treePtr,int *index, value_t *return_val);
int searchAll(bplus_tree *treePtr,int *i_start, int *i_end);
int updateRecord(bplus_tree *treePtr,int *key, value_t *values);
void printTable(int *index, value_t *values);
void intToKeyT(bpt::key_t *a,int *b);
bool is_file_exist(const char *fileName);
double durationTime(clock_t *f,clock_t *s);


bplus_tree *duck_db_ptr;

//初始化操作
void initialSystem(){
	//帮助文档及初始界面
	printHelpMess();
	//初始化数据库
	bplus_tree duck_db(dbFileName, (!is_file_exist(dbFileName)));
	duck_db_ptr = &duck_db;
	//开始输入命令
	selectCommand();	
}

//欢迎界面
void printHelpMess(){
	cout << "*********************************************************************************************"<<endl<<endl
		<<" 				Welcome to the BPT_db                 "<<endl
		<<"*********************************************************************************************"<<endl
		<<"  .help 							print help message;"<<endl
		<<"  .exit 							exit program;"<<endl
		<<"  insert db {index} {name} {stu_id} {class_id};"<<endl
		<<"  delete from db where id = {index};"<<endl
		<<"  update db {name} {stu_id} {class_id} where id = {index};"<<endl
		<<"  select * from db where id = {index};"<<endl
		<<"  select * from db where id in ({minIndex},{maxIndex});"<<endl
		<<"*********************************************************************************************"<<endl
		<<endl << nextLineHeader;
}

//输入命令界面
void selectCommand(){
	char *userCommand = new char[256];	
	while(true){
	    //输入一串字符串然后进行合法性比较
	    cin.getline(userCommand,256);		
	    //strcmp函数比较命令
	    if(strcmp(userCommand,".exit") == 0){
	    	cout << exitMessage;
		break;
	    }
	    else if(strcmp(userCommand,".help") == 0){
	    	printHelpMess();
	    }
	    //如果是非退出和帮助命令，只需要利用strncmp函数比较前6个字符
	    else if(strncmp(userCommand,"insert",6) == 0){
	    	int *keyIndex = new int;
	    	value_t *insertData = new value_t;
		//利用sscanf比较输入变量是否合法
	    	int okNum = sscanf(userCommand,"insert db %d %s %d %s;", 
	    		keyIndex, insertData->name,&(insertData->stu_id),insertData->class_id);

		if(okNum < 3){//输入不合法
			cout << errorMessage<< nextLineHeader;
		}
		else{
        		startTime = clock(); 
			//利用B+树插入数据
			int return_code = insertRecord(duck_db_ptr,keyIndex,insertData);
			finishTime = clock(); 
			//分类，查找成功，失败，以及异常
			if (return_code == 0){
				cout << "> executed insert index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
			}
			else if(return_code == 1){
				cout << "> failed: already exist index:"<<   *keyIndex << "\n"<< nextLineHeader;
			}
			else{
				cout << "> failed!\n"<< nextLineHeader;
			}
		}

	    }
	    else if(strncmp(userCommand,"delete",6) == 0){
	    	int *keyIndex = new int;
	    	int okNum = sscanf(userCommand,"delete from db where id = %d;", keyIndex);
		if(okNum < 1){
			cout << errorMessage<< nextLineHeader;
		}
		else{
			startTime = clock(); 
			//利用B+树删除数据
			int return_code = deleteRecord(duck_db_ptr,keyIndex);
			finishTime = clock(); 
			if (return_code == 0){
				cout << "> executed delete index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
			}else if(return_code == -1){
				cout << "> failed ! no index:"<<   *keyIndex << "\n"<< nextLineHeader;
			}else{
				cout << "> failed!\n"<< nextLineHeader;
			}
		}

	    }
	    else if(strncmp(userCommand,"select",6) == 0){
		//select需要区分范围查询和单点查询
	    	if(!strstr (userCommand,"=")){
	    		int i_start,i_end;//范围查找
	    		int okNum = sscanf(userCommand,"select * from db where id in (%d,%d);", &i_start,&i_end);
			if(okNum < 2){
				cout << errorMessage<< nextLineHeader;
			}else{
				startTime = clock(); 
				searchAll(duck_db_ptr,&i_start, &i_end);
				finishTime = clock(); 
				cout << "> executed search, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
			}

	    	}
		else{
		    	int *keyIndex = new int;
		    	int okNum = sscanf(userCommand,"select * from db where id = %d;", keyIndex);
			if(okNum < 1){
				cout << errorMessage<< nextLineHeader;
			}
			else{
				value_t *return_val = new value_t;
				startTime = clock(); 
				int return_code = searchRecord(duck_db_ptr,keyIndex,return_val);
				finishTime = clock(); 	    	
				if (return_code != 0){
					cout << "> index:"<< *keyIndex << " doesn't exist, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
				}
				else{
					printTable( keyIndex , return_val);
					cout << "> executed search, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
						
				}
			}
		}

	    }
	    //更新操作
	    else if(strncmp(userCommand,"update",6) == 0){
	    	int *keyIndex = new int;
	    	value_t *updateData = new value_t;
	    	int okNum = sscanf(userCommand,"update db %s %d %s where id = %d;", 
	    		 updateData->name,&(updateData->stu_id),updateData->class_id,keyIndex);
		if(okNum < 3){
			cout << errorMessage<< nextLineHeader;
		}
		else{
			startTime = clock(); 
			int return_code = updateRecord(duck_db_ptr,keyIndex,updateData);
			finishTime = clock(); 
			if (return_code == 0){
				cout << "> executed update index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
			}else{
				cout << "> failed! no index:"<<   *keyIndex << ", time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
			}
		}

	    }
	    else{
	    	cout << errorMessage<< nextLineHeader;
	    }
	}

}

//B+树的插入操作
int insertRecord(bplus_tree *treePtr,int *index, value_t *values){	
	bpt::key_t key;
	intToKeyT(&key,index);
	return (*treePtr).insert(key, *values);
}

//删除操作 
int deleteRecord(bplus_tree *treePtr,int *index){	
	bpt::key_t key;
	intToKeyT(&key,index);
	return (*treePtr).remove(key);
}

//更新操作
int updateRecord(bplus_tree *treePtr,int *index, value_t *values){
	bpt::key_t key;
	intToKeyT(&key,index);
	return (*treePtr).update(key, *values);
}

//单值查询
int searchRecord(bplus_tree *treePtr,int *index, value_t *return_val){
	bpt::key_t key;
	intToKeyT(&key,index);
	return (*treePtr).search(key, return_val); 
}

//范围查找
int searchAll(bplus_tree *treePtr,int *start, int *end){
        //现在需要使用TextTable.h中的制表操作来显示数据表
    	TextTable t( '-', '|', '+' );
    	t.add( " id " );
    	t.add( " name " );
    	t.add( " stu_id " );
    	t.add( " class_id " );
    	t.endOfRow();
	bpt::key_t key;
	value_t *return_val = new value_t;

	for (int i = *start; i <= *end; ++i){		
		intToKeyT(&key,&i);		
		int return_code = (*treePtr).search(key, return_val);
		switch(return_code){
			case -1:
			//没有这个值
			    break;
			case 0:
			//有值，插入表中
			    t.add( to_string(i) );
			    t.add( return_val ->name );
			    t.add( to_string(return_val ->stu_id));
			    t.add( return_val ->class_id );
			    t.endOfRow();
			    break;
			case 1:
			//删除
			    break;
		}		

	}
	cout << t << endl;
	
}

//打印数据表
void printTable(int *index, value_t *values){
	TextTable t( '-', '|', '+' );

    	t.add( " id " );
    	t.add( " name " );
    	t.add( " stu_id " );
    	t.add( " class_id " );
    	t.endOfRow();

    	t.add( to_string(*index) );
    	t.add( values ->name );
    	t.add( to_string(values ->stu_id));
    	t.add( values ->class_id );
    	t.endOfRow();
    	cout << t << endl;
}
//int to key_t，类似itos函数
void intToKeyT(bpt::key_t *a,int *b){
	char key[16] = { 0 };
	sprintf(key, "%d", *b);
	*a = key;
}

//判断数据文件是否存在
bool is_file_exist(const char *fileName){
    	ifstream ifile(fileName);
  	return ifile.good();
}

//计算命令时间
double durationTime(clock_t *f,clock_t *s){
	return (double)(*f - *s) / CLOCKS_PER_SEC;	
}

//主函数
int main(int argc, char *argv[]){
	initialSystem();
}



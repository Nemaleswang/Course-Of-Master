#include "../include/bpt.h"
#include "../include/TextTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <iostream>
#include <fstream>

using namespace bpt;
using namespace std;

const char *errorMessage = "> 输入非法,输入 \"help\" 查看帮助文档!\n";
const char *nextLineHeader ="> ";
const char *exitMessage = "> 成功退出!\n";
const char *dbFileName = "./data/bpt_db.bin";//数据存储位置


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
	   cout <<"------------------------------------------------------------------------------"<<endl<<endl
		<<" 			  欢迎界面/帮助文档               "<<endl<<endl
		<<" 	  按格式输入以下字符串即可实现CRUD命令，中括号中的数据需要自己定义            "<<endl
		<<"------------------------------------------------------------------------------"<<endl
		<<"  help \n  键入此命令再次显示该界面"<<endl<<endl
		<<"  exit \n  退出数据库操作界面"<<endl<<endl
		<<"  insert into bpt_db [index] [name] [stu_id] [class_id] \n  插入命令,输入四个值，分别为id，姓名，学号，班号"<<endl<<endl
		<<"  delete from bpt_db where id = [index] \n  删除命令，只需要id"<<endl<<endl
		<<"  update bpt_db [name] [stu_id] [class_id] where id = [index] \n  更新命令，输入四个值，分别为id，姓名，学号，班号"<<endl<<endl
		<<"  select * from bpt_db where id = [index] \n  查找命令，只需要输入id"<<endl<<endl
		<<"------------------------------------------------------------------------------"<<endl
		<<endl << nextLineHeader;
}

//输入命令界面
void selectCommand(){
	char *userCommand = new char[256];	
	while(true){
	    //输入一串字符串然后进行合法性比较
	    cin.getline(userCommand,256);		
	    //strcmp函数比较命令
	    if(strcmp(userCommand,"exit") == 0){
	    	cout << exitMessage;
		break;
	    }
	    else if(strcmp(userCommand,"help") == 0){
	    	printHelpMess();
	    }
	    //如果是非退出和帮助命令，只需要利用strncmp函数比较前6个字符
	    else if(strncmp(userCommand,"insert",6) == 0){
	    	int *keyIndex = new int;
	    	value_t *insertData = new value_t;
		//利用sscanf比较输入变量是否合法
	    	int okNum = sscanf(userCommand,"insert into bpt_db %d %s %d %s;", 
	    		keyIndex, insertData->name,&(insertData->stu_id),insertData->class_id);

		if(okNum < 3){//输入不合法
			cout << errorMessage<< nextLineHeader;
		}
		else{
			//利用B+树插入数据
			int return_code = insertRecord(duck_db_ptr,keyIndex,insertData);
			//分类，查找成功，失败，以及异常
			if (return_code == 0){
				cout << "> 插入id成功:"<<   *keyIndex << "\n"<< nextLineHeader;
			}
			else if(return_code == 1){
				cout << "> 插入失败: 该id已经存在:"<<   *keyIndex << "\n"<< nextLineHeader;
			}
			else{
				cout << "> 失败!\n"<< nextLineHeader;
			}
		}

	    }
	    else if(strncmp(userCommand,"delete",6) == 0){
	    	int *keyIndex = new int;
	    	int okNum = sscanf(userCommand,"delete from bpt_db where id = %d;", keyIndex);
		if(okNum < 1){
			cout << errorMessage<< nextLineHeader;
		}
		else{
			//利用B+树删除数据
			int return_code = deleteRecord(duck_db_ptr,keyIndex);
			if (return_code == 0){
				cout << "> 删除id成功:"<<   *keyIndex << "\n"<< nextLineHeader;
			}else if(return_code == -1){
				cout << "> 删除失败，输入非法或该id不存在:"<<   *keyIndex << "\n"<< nextLineHeader;
			}else{
				cout << "> 失败!\n"<< nextLineHeader;
			}
		}

	    }
	    else if(strncmp(userCommand,"select",6) == 0){
		//select单点查询
		int *keyIndex = new int;
		int okNum = sscanf(userCommand,"select * from bpt_db where id = %d;", keyIndex);
		if(okNum < 1){
			cout << errorMessage<< nextLineHeader;
		}
		else{
			value_t *return_val = new value_t;
			int return_code = searchRecord(duck_db_ptr,keyIndex,return_val);	
			if (return_code != 0){
				cout << "> id:"<< *keyIndex << " 不存在或者输入非法!\n"<< nextLineHeader;
			}
			else{
				printTable( keyIndex , return_val);
				cout << "> 索引查找成功\n"<< nextLineHeader;
						
			}
		}

	    }
	    //更新操作
	    else if(strncmp(userCommand,"update",6) == 0){
	    	int *keyIndex = new int;
	    	value_t *updateData = new value_t;
	    	int okNum = sscanf(userCommand,"update bpt_db %s %d %s where id = %d;", 
	    		 updateData->name,&(updateData->stu_id),updateData->class_id,keyIndex);
		if(okNum < 3){
			cout << errorMessage<< nextLineHeader;
		}
		else{
			int return_code = updateRecord(duck_db_ptr,keyIndex,updateData);
			if (return_code == 0){
				cout << "> id更新成功:"<<   *keyIndex << "\n"<< nextLineHeader;
			}else{
				cout << "> 失败! 输入非法或更新的id不存在:"<<   *keyIndex << "\n"<< nextLineHeader;
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

//主函数
int main(int argc, char *argv[]){
	initialSystem();
}



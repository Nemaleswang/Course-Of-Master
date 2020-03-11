#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <string.h>

namespace bpt{
	//定义B+树的阶数
	#define BP_ORDER 10
	
	//B+树节点的定义
	struct value_t{
		char name[256];//姓名
		int stu_id;//学号
		char class_id[256];//班号
	};

	struct key_t{
		char id[16];
		//初始化
		key_t(const char *str = ""){
			bzero(id,sizeof(id));
			strcpy(id,str);
		}
	};
	
	//id大小比较
	inline int keycmp(const key_t &a,const key_t &b){
		int x = strlen(a.id)-strlen(b.id);
		return x == 0?strcmp(a.id,b.id):x;
	}
	
	#define OPERATOR_KEYCMP(type) \
    	bool operator< (const key_t &l, const type &r) {\
        	return keycmp(l, r.key) < 0;\
	    }\
	bool operator< (const type &l, const key_t &r) {\
        	return keycmp(l.key, r) < 0;\
	}\
	bool operator== (const key_t &l, const type &r) {\
    		return keycmp(l, r.key) == 0;\
	}\
	bool operator== (const type &l, const key_t &r) {\
        	return keycmp(l.key, r) == 0;\
	}

}
#endif

#ifndef BPT_H
#define BPT_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "predefined.h"

namespace bpt{
	//初始偏移量
	#define OFFSET_META 0
	//一个块的偏移量
	#define OFFSET_BLOCK OFFSET_META + sizeof(meta_t)
	//叶子节点的偏移量
	#define SIZE_NO_CHILDREN sizeof(leaf_node_t) - BP_ORDER * sizeof(record_t)
	
	//B+树的构造
	typedef struct {
	    size_t order; //B+树的阶
	    size_t value_size; //value的size
	    size_t key_size;   //key的size
	    size_t internal_node_num; //有多少非叶子节点
	    size_t leaf_node_num;     //叶子数量
	    size_t height;            //树高
	    off_t slot;        //新块的存储偏移量
	    off_t root_offset; //根节点的偏移量
	    off_t leaf_offset; //第一个叶子的偏移量
	} meta_t;
	
	
	struct index_t{
	    key_t key;
	    off_t child;//非叶子节点有孩子
	};

	//非叶子节点的定义
	struct internal_node_t{
	    typedef index_t *child_t;
	    off_t parent; //父亲节点偏移量
	    off_t next; //下一个叶子节点偏移量
	    off_t prev;//上一个叶子节点偏移量
	    size_t n;
   	    index_t children[BP_ORDER];
	};

	struct record_t {
	    key_t key;
	    value_t value;//叶子节点没有孩子
	};
	
	//叶子节点的定义
	struct leaf_node_t {
	    typedef record_t *child_t;
	    off_t parent; //父亲节点偏移量
	    off_t next; //下一个叶子节点偏移量
	    off_t prev;//上一个叶子节点偏移量
	    size_t n;
	    	record_t children[BP_ORDER];
	};

	//B+树的函数封装
	class bplus_tree {
	public:
    		bplus_tree(const char *path, bool force_empty = false);

	    	//固定键值查找
    		int search(const key_t& key, value_t *value) const;
		
		//范围查找
		int search_range(key_t *left, const key_t &right,
                     value_t *values, size_t max, bool *next = NULL) const;
		
		//删除操作
		int remove(const key_t& key);
		
		//插入操作
		int insert(const key_t& key, value_t value);
		
		//更新操作
		int update(const key_t& key, value_t value);

		
		meta_t get_meta() const {
			return meta;
	    	};

		char path[512];
		meta_t meta;

		//初始化一颗空树
		void init_from_empty();

		//按键值寻找偏移量
		off_t search_index(const key_t &key) const;

		//寻找叶子便宜量
		off_t search_leaf(off_t index, const key_t &key) const;
		off_t search_leaf(const key_t &key) const{
			return search_leaf(search_index(key), key);
		}

		//删除非叶子节点
		void remove_from_index(off_t offset, internal_node_t &node,
				           const key_t &key);

		//从非叶子节点借值
		bool borrow_key(bool from_right, internal_node_t &borrower,
				    off_t offset);

		//从叶子节点借值
		bool borrow_key(bool from_right, leaf_node_t &borrower);

		//改变某节点的父亲键值
		void change_parent_child(off_t parent, const key_t &o, const key_t &n);

		//合并叶子节点
		void merge_leafs(leaf_node_t *left, leaf_node_t *right);
		void merge_keys(index_t *where, internal_node_t &left,
				    internal_node_t &right);

		//插入叶子节点
		void insert_record_no_split(leaf_node_t *leaf,
				                const key_t &key, const value_t &value);

		//增加非叶子节点
		void insert_key_to_index(off_t offset, const key_t &key,
				             off_t value, off_t after);
		void insert_key_to_index_no_split(internal_node_t &node, const key_t &key,
				                      off_t value);

		//更改某孩子的父亲
		void reset_index_children_parent(index_t *begin, index_t *end,
				                     off_t parent);

		//创建节点
		template<class T>
		void node_create(off_t offset, T *node, T *next);
		
		//删除节点
		template<class T>
		void node_remove(T *prev, T *node);

		//多级文件的打开和关闭
		mutable FILE *fp;
		mutable int fp_level;
		void open_file(const char *mode = "rb+") const{
			if (fp_level == 0)
			fp = fopen(path, mode);
			++fp_level;
		}

		void close_file() const{
			if (fp_level == 1)
			fclose(fp);
			--fp_level;
		}

		//从磁盘分配空间
		off_t alloc(size_t size){
			off_t slot = meta.slot;
			meta.slot += size;
			return slot;
		}

		off_t alloc(leaf_node_t *leaf){
			leaf->n = 0;
			meta.leaf_node_num++;
			return alloc(sizeof(leaf_node_t));
		}

		off_t alloc(internal_node_t *node){
			node->n = 1;
			meta.internal_node_num++;
			return alloc(sizeof(internal_node_t));
		}

		//从磁盘删除数据
		void unalloc(leaf_node_t *leaf, off_t offset){
			--meta.leaf_node_num;
		}

		void unalloc(internal_node_t *node, off_t offset){
			--meta.internal_node_num;
		}

		//从磁盘读入块
		int map(void *block, off_t offset, size_t size) const{
			open_file();
			fseek(fp, offset, SEEK_SET);
			size_t rd = fread(block, size, 1, fp);
			close_file();
			return rd - 1;
		}

		template<class T>
		int map(T *block, off_t offset) const{
			return map(block, offset, sizeof(T));
		}

		//从磁盘写入块
		int unmap(void *block, off_t offset, size_t size) const{
			open_file();
			fseek(fp, offset, SEEK_SET);
			size_t wd = fwrite(block, size, 1, fp);
			close_file();	
			return wd - 1;
		}
		template<class T>
		int unmap(T *block, off_t offset) const{
			return unmap(block, offset, sizeof(T));
		}

	};
}
#endif

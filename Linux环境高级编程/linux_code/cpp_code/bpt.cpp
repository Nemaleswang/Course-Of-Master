#include "../include/bpt.h"

#include <stdlib.h>

#include <list>
#include <algorithm>
using std::swap;
using std::binary_search;
using std::lower_bound;
using std::upper_bound;

namespace bpt{
	OPERATOR_KEYCMP(index_t)
	OPERATOR_KEYCMP(record_t)

	//一个节点的起始偏移量以及结束偏移量
	template<class T>
	inline typename T::child_t begin(T &node) {
		return node.children; 
	}
	template<class T>
	inline typename T::child_t end(T &node) {
		return node.children + node.n;
	}
	
	//利用二分搜索
	inline index_t *find(internal_node_t &node, const key_t &key) {
    		return upper_bound(begin(node), end(node) - 1, key);
	}	
	inline record_t *find(leaf_node_t &node, const key_t &key) {
   		return lower_bound(begin(node), end(node), key);
	}	

	bplus_tree::bplus_tree(const char *p, bool force_empty) : fp(NULL), fp_level(0){
	    	bzero(path, sizeof(path));
	    	strcpy(path, p);
	    	if (!force_empty){
			if (map(&meta, OFFSET_META) != 0)
		    		force_empty = true;
		}

	   	if (force_empty) {
			open_file("w+"); 
			//初始化一棵树
			init_from_empty();
			close_file();
	    	}
	}
	int bplus_tree::search(const key_t& key, value_t *value) const{
	    	leaf_node_t leaf;
	    	map(&leaf, search_leaf(key));
	   	//寻找记录
	   	record_t *record = find(leaf, key);
	    	if (record != leaf.children + leaf.n) {
			*value = record->value;
			return keycmp(record->key, key);
	    	} 
		else {
			return -1;
	    	}
	}

	int bplus_tree::search_range(key_t *left, const key_t &right,
                             value_t *values, size_t max, bool *next) const{
		//非法范围	
	    	if (left == NULL || keycmp(*left, right) > 0)
			return -1;
		//分别寻找left和right的offset
	    	off_t off_left = search_leaf(*left);
	    	off_t off_right = search_leaf(right);
	    	off_t off = off_left;
	    	size_t i = 0;
	    	record_t *b, *e;

	    	leaf_node_t leaf;
	    	while (off != off_right && off != 0 && i < max) {
			map(&leaf, off);

			//起始点
			if (off_left == off) b = find(leaf, *left);
			else b = begin(leaf);

			//复制值
			e = leaf.children + leaf.n;
			for (; b != e && i < max; ++b, ++i)
		    		values[i] = b->value;

			off = leaf.next;
	    	}

	    	//最后一个叶子节点
	    	if (i < max) {
			map(&leaf, off_right);

			b = find(leaf, *left);
			e = upper_bound(begin(leaf), end(leaf), right);
			for (; b != e && i < max; ++b, ++i)
		    		values[i] = b->value;
	    	}

	    	//为下一次迭代做标记
	    	if (next != NULL) {
			if (i == max && b != e) {
		    		*next = true;
		    		*left = b->key;
			} 
 			else {
		    		*next = false;
			}
	    	}

	    	return i;
	}
	int bplus_tree::remove(const key_t& key){
	    	internal_node_t parent;
	    	leaf_node_t leaf;

	    	//寻找父亲节点
	    	off_t parent_off = search_index(key);
	    	map(&parent, parent_off);

	    	//寻找当前节点
	    	index_t *where = find(parent, key);
	    	off_t offset = where->child;
	    	map(&leaf, offset);

	    	//对节点存在性做验证
	    	if (!binary_search(begin(leaf), end(leaf), key))
			return -1;

	    
	    	size_t min_n = meta.leaf_node_num == 1 ? 0 : meta.order / 2;
	    	assert(leaf.n >= min_n && leaf.n <= meta.order);

	    	//删除节点
	    	record_t *to_delete = find(leaf, key);
	    	std::copy(to_delete + 1, end(leaf), to_delete);
	    	leaf.n--;

	    	//合并节点
	    	if (leaf.n < min_n) {
			//左边
			bool borrowed = false;
			if (leaf.prev != 0)
		    		borrowed = borrow_key(false, leaf);
			//右边
			if (!borrowed && leaf.next != 0)
		    		borrowed = borrow_key(true, leaf);
			//合并
			if (!borrowed) {
		    		assert(leaf.next != 0 || leaf.prev != 0);
		    		key_t index_key;
		    		if (where == end(parent) - 1) {
		        		//如果该叶子是最后一个元素
		        		assert(leaf.prev != 0);
		        		leaf_node_t prev;
		        		map(&prev, leaf.prev);
		        		index_key = begin(prev)->key;

		        		merge_leafs(&prev, &leaf);
		        		node_remove(&prev, &leaf);
		        		unmap(&prev, leaf.prev);
		    		} 
				else {
		        		//不是最后一个元素
		        		assert(leaf.next != 0);
		        		leaf_node_t next;
		        		map(&next, leaf.next);
		        		index_key = begin(leaf)->key;

		        		merge_leafs(&leaf, &next);
		        		node_remove(&leaf, &next);
		        		unmap(&leaf, offset);
		    		}
				//移除父亲节点
		    		remove_from_index(parent_off, parent, index_key);
			} 
			else {
		    		unmap(&leaf, offset);
			}
	    	} 
		else {
			unmap(&leaf, offset);
	   	}
	    	return 0;
	}
	//插入操作
	int bplus_tree::insert(const key_t& key, value_t value){
    		off_t parent = search_index(key);
    		off_t offset = search_leaf(parent, key);
    		leaf_node_t leaf;
    		map(&leaf, offset);

    		//检查是否具有相同的key
    		if (binary_search(begin(leaf), end(leaf), key)) return 1;
    		if (leaf.n == meta.order) {
        		//叶子满的时候需要分裂节点

        		//创建一个新的兄弟节点
        		leaf_node_t new_leaf;
        		node_create(offset, &leaf, &new_leaf);

        		//寻找均匀点进行分裂
        		size_t point = leaf.n / 2;
        		bool place_right = keycmp(key, leaf.children[point].key) > 0;
        		if (place_right) ++point;

        		//分裂节点
        		std::copy(leaf.children + point, leaf.children + leaf.n,
                  		new_leaf.children);
        		new_leaf.n = leaf.n - point;
        		leaf.n = point;
        		// 放入新的值
        		if (place_right)
            			insert_record_no_split(&new_leaf, key, value);
        		else
            			insert_record_no_split(&leaf, key, value);

        		unmap(&leaf, offset);
        		unmap(&new_leaf, leaf.next);

        		//插入新节点
        		insert_key_to_index(parent, new_leaf.children[0].key,
                            		offset, leaf.next);
    		     } 
		else {
        		insert_record_no_split(&leaf, key, value);
        		unmap(&leaf, offset);
    		}
    		return 0;
	}
	
	//B+树的更新操作
	int bplus_tree::update(const key_t& key, value_t value)	{
	    off_t offset = search_leaf(key);
	    leaf_node_t leaf;
	    map(&leaf, offset);	    
		
	    record_t *record = find(leaf, key);
	    //该值是存在于B+树中的
	    if (record != leaf.children + leaf.n){
		if (keycmp(key, record->key) == 0) {
		    record->value = value;
		    unmap(&leaf, offset);
		    return 0;
		} 
		else {
		    return 1;
		}
	    }	
	    else return -1;
	}
	//从索引中删值
	void bplus_tree::remove_from_index(off_t offset, internal_node_t &node,
		                           const key_t &key){
	    size_t min_n = meta.root_offset == offset ? 1 : meta.order / 2;
	    assert(node.n >= min_n && node.n <= meta.order);

	    //移除key
	    key_t index_key = begin(node)->key;
	    index_t *to_delete = find(node, key);
	    if (to_delete != end(node)) {
		(to_delete + 1)->child = to_delete->child;
		std::copy(to_delete + 1, end(node), to_delete);
	    }
	    node.n--;

	    if (node.n == 1 && meta.root_offset == offset &&
		               meta.internal_node_num != 1) {
		unalloc(&node, meta.root_offset);
		meta.height--;
		meta.root_offset = node.children[0].child;
		unmap(&meta, OFFSET_META);
		return;
	    }

	    //合并操作
	    if (node.n < min_n) {
		internal_node_t parent;
		map(&parent, node.parent);

		//第一次从左边借
		bool borrowed = false;
		if (offset != begin(parent)->child)
		    borrowed = borrow_key(false, node, offset);

		//然后移到右边
		if (!borrowed && offset != (end(parent) - 1)->child)
		    borrowed = borrow_key(true, node, offset);

		//合并
		if (!borrowed) {
		    assert(node.next != 0 || node.prev != 0);

		    if (offset == (end(parent) - 1)->child) {
		        //叶子是最后一个元素，与左边合并
		        assert(node.prev != 0);
		        internal_node_t prev;
		        map(&prev, node.prev);

		        index_t *where = find(parent, begin(prev)->key);
		        reset_index_children_parent(begin(node), end(node), node.prev);
		        merge_keys(where, prev, node);
		        unmap(&prev, node.prev);
		    } 
		    else {
		        //与右边合并
		        assert(node.next != 0);
		        internal_node_t next;
		        map(&next, node.next);

		        index_t *where = find(parent, index_key);
		        reset_index_children_parent(begin(next), end(next), offset);
		        merge_keys(where, node, next);
		        unmap(&node, offset);
		    }

		    //移除父亲节点的key
		    remove_from_index(node.parent, parent, index_key);
		} 
		else {
		    unmap(&node, offset);
		}
	    } 
	    else {
		unmap(&node, offset);
	    }
	}

	//borrow操作
	bool bplus_tree::borrow_key(bool from_right, internal_node_t &borrower,
		                    off_t offset)	{
	    	typedef typename internal_node_t::child_t child_t;
		
		//选择左边或者右边
	   	off_t lender_off = from_right ? borrower.next : borrower.prev;
	    	internal_node_t lender;
	    	map(&lender, lender_off);

	    	assert(lender.n >= meta.order / 2);
	    	if (lender.n != meta.order / 2) {
			child_t where_to_lend, where_to_put;

			internal_node_t parent;

			//交换键值,分类为从右借和从左借
			if (from_right) {
				//定位交换的位置
		    		where_to_lend = begin(lender);
		    		where_to_put = end(borrower);

		    		map(&parent, borrower.parent);
		    		child_t where = lower_bound(begin(parent), end(parent) - 1,
		                                (end(borrower) -1)->key);
		    		where->key = where_to_lend->key;
		    		unmap(&parent, borrower.parent);
			} 
			else {
		    		where_to_lend = end(lender) - 1;
		    		where_to_put = begin(borrower);

		    		map(&parent, lender.parent);
		    		child_t where = find(parent, begin(lender)->key);
		    		where_to_put->key = where->key;
		    		where->key = (where_to_lend - 1)->key;
		    		unmap(&parent, lender.parent);
			}

			//存储数据
			std::copy_backward(where_to_put, end(borrower), end(borrower) + 1);
			*where_to_put = *where_to_lend;
			borrower.n++;

			//删除数据
			reset_index_children_parent(where_to_lend, where_to_lend + 1, offset);
			std::copy(where_to_lend + 1, end(lender), where_to_lend);
			lender.n--;
			unmap(&lender, lender_off);
			return true;
	    	}

	    return false;
	}

	bool bplus_tree::borrow_key(bool from_right, leaf_node_t &borrower){

		//选择左边或者右边
    		off_t lender_off = from_right ? borrower.next : borrower.prev;
    		leaf_node_t lender;
    		map(&lender, lender_off);

    		assert(lender.n >= meta.order / 2);
    		if (lender.n != meta.order / 2) {
        		typename leaf_node_t::child_t where_to_lend, where_to_put;

        		//交换键值,分类为从右借和从左借
        		if (from_right) {
            			where_to_lend = begin(lender);
            			where_to_put = end(borrower);
            			change_parent_child(borrower.parent, begin(borrower)->key,
                                	lender.children[1].key);
        		} 
			else {
            			where_to_lend = end(lender) - 1;
            			where_to_put = begin(borrower);
            			change_parent_child(lender.parent, begin(lender)->key,
                                	where_to_lend->key);
        		}

        		//存储数据
        		std::copy_backward(where_to_put, end(borrower), end(borrower) + 1);
        		*where_to_put = *where_to_lend;
        		borrower.n++;

        		///删除数据
        		std::copy(where_to_lend + 1, end(lender), where_to_lend);
        		lender.n--;
        		unmap(&lender, lender_off);
        		return true;
    		}

    		return false;
	}
	
	//删节点需要更改父亲位置
	void bplus_tree::change_parent_child(off_t parent, const key_t &o,
                                     const key_t &n){
	    	internal_node_t node;
	    	map(&node, parent);

	    	index_t *w = find(node, o);
	    	assert(w != node.children + node.n); 

	    	w->key = n;
	    	unmap(&node, parent);
	    	if (w == node.children + node.n - 1) {
			//迭代修改
			change_parent_child(node.parent, o, n);
		}
	}
	
	//叶子节点合并
	void bplus_tree::merge_leafs(leaf_node_t *left, leaf_node_t *right){
    		std::copy(begin(*right), end(*right), end(*left));
    		left->n += right->n;
	}
	
	//合并关键值
	void bplus_tree::merge_keys(index_t *where,
                 	internal_node_t &node, internal_node_t &next){
    		std::copy(begin(next), end(next), end(node));
    		node.n += next.n;
    		node_remove(&node, &next);
	}
	
	//插入记录
	void bplus_tree::insert_record_no_split(leaf_node_t *leaf,
                                        const key_t &key, const value_t &value){
		record_t *where = upper_bound(begin(*leaf), end(*leaf), key);
		std::copy_backward(where, end(*leaf), end(*leaf) + 1);

		where->key = key;
		where->value = value;
		leaf->n++;
	}
	
	//插值
	void bplus_tree::insert_key_to_index(off_t offset, const key_t &key,
                                     off_t old, off_t after){
		//初始为空的情况
    		if (offset == 0) {
        		//建立新的根节点
        		internal_node_t root;
        		root.next = root.prev = root.parent = 0;
        		meta.root_offset = alloc(&root);
        		meta.height++;
			//更新偏移量
			root.n = 2;
			root.children[0].key = key;
			root.children[0].child = old;
			root.children[1].child = after;
			unmap(&meta, OFFSET_META);
			unmap(&root, meta.root_offset);
			//更新父亲节点
			reset_index_children_parent(begin(root), end(root),
				                    meta.root_offset);
			return;
		}

	    	internal_node_t node;
	    	map(&node, offset);
	    	assert(node.n <= meta.order);

	    	if (node.n == meta.order) {
			//孩子节点满了需要进行拆分
			internal_node_t new_node;
			node_create(offset, &node, &new_node);

			//在均匀点进行分裂
			size_t point = (node.n - 1) / 2;
			bool place_right = keycmp(key, node.children[point].key) > 0;
			if (place_right) ++point;

			//防止寻找过头
			if (place_right && keycmp(key, node.children[point].key) < 0)
		    		point--;

        		key_t middle_key = node.children[point].key;

        		//分裂节点
        		std::copy(begin(node) + point + 1, end(node), begin(new_node));
        		new_node.n = node.n - point - 1;
        		node.n = point + 1;

        		//将新的值加入树中
        		if (place_right)
            			insert_key_to_index_no_split(new_node, key, after);
        		else
            			insert_key_to_index_no_split(node, key, after);

        		unmap(&node, offset);
        		unmap(&new_node, node.next);

        		//更新父亲节点
        		reset_index_children_parent(begin(new_node), end(new_node), node.next);
        		insert_key_to_index(node.parent, middle_key, offset, node.next);
    		} 
		//孩子没满，直接插入
		else {
        		insert_key_to_index_no_split(node, key, after);
        		unmap(&node, offset);
    		}
	}
	//插节点
	void bplus_tree::insert_key_to_index_no_split(internal_node_t &node,
                                              const key_t &key, off_t value){
	    	index_t *where = upper_bound(begin(node), end(node) - 1, key);
	    	//索引后移
	    	std::copy_backward(where, end(node), end(node) + 1);
	    	//插值
	    	where->key = key;
	    	where->child = (where + 1)->child;
	    	(where + 1)->child = value;
	    	node.n++;
	}

	void bplus_tree::reset_index_children_parent(index_t *begin, index_t *end,
                                             off_t parent){
    		//改变叶子节点或者非叶子节点的父亲节点
    		internal_node_t node;
   		while (begin != end) {
        		map(&node, begin->child);
        		node.parent = parent;
        		unmap(&node, begin->child, SIZE_NO_CHILDREN);
        		++begin;
    		}
	}
	//搜值
	off_t bplus_tree::search_index(const key_t &key) const{
    		off_t org = meta.root_offset;
    		int height = meta.height;
    		while (height > 1) {
        		internal_node_t node;
        		map(&node, org);

        		index_t *i = upper_bound(begin(node), end(node) - 1, key);
        		org = i->child;
        		--height;
    		}

    		return org;
	}
	//查找叶子节点
	off_t bplus_tree::search_leaf(off_t index, const key_t &key) const{
    		internal_node_t node;
    		map(&node, index);

    		index_t *i = upper_bound(begin(node), end(node) - 1, key);
    		return i->child;
	}
	//创建节点和删除节点的方式类比链表
	//创建新节点
	template<class T>
	void bplus_tree::node_create(off_t offset, T *node, T *next){
    		//新的同级节点
    		next->parent = node->parent;
    		next->next = node->next;
    		next->prev = offset;
    		node->next = alloc(next);
    		//更新下一个节点的前驱
    		if (next->next != 0) {
        		T old_next;
        		map(&old_next, next->next, SIZE_NO_CHILDREN);
        		old_next.prev = node->next;
        		unmap(&old_next, next->next, SIZE_NO_CHILDREN);
    		}
    		unmap(&meta, OFFSET_META);
	}
	
	//删节点
	template<class T>
	void bplus_tree::node_remove(T *prev, T *node){
    		unalloc(node, prev->next);
    		prev->next = node->next;
    		if (node->next != 0) {
        		T next;
        		map(&next, node->next, SIZE_NO_CHILDREN);
        		next.prev = node->prev;
        		unmap(&next, node->next, SIZE_NO_CHILDREN);
    		}
    		unmap(&meta, OFFSET_META);
	}
	//初始化一颗空树
	void bplus_tree::init_from_empty(){
	    	//初始化一个B+树的元数据
    		bzero(&meta, sizeof(meta_t));
    		meta.order = BP_ORDER;
    		meta.value_size = sizeof(value_t);
    		meta.key_size = sizeof(key_t);
    		meta.height = 1;
    		meta.slot = OFFSET_BLOCK;

    		//初始化根节点
    		internal_node_t root;
    		root.next = root.prev = root.parent = 0;
    		meta.root_offset = alloc(&root);

    		//初始化空的叶子
    		leaf_node_t leaf;
    		leaf.next = leaf.prev = 0;
    		leaf.parent = meta.root_offset;
    		meta.leaf_offset = root.children[0].child = alloc(&leaf);

    		unmap(&meta, OFFSET_META);
    		unmap(&root, meta.root_offset);
    		unmap(&leaf, root.children[0].child);
	}
}

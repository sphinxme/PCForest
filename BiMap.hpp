
#ifndef BIMAP_HPP
#define BIMAP_HPP

#include <initializer_list>
#include <unordered_map>
#include <vector>
#include <list>

using std::initializer_list;
using std::unordered_map;
using std::vector;
using std::list;


template <typename T> struct BiMapNode;			// 节点类
template <typename T> class  BiMap;				// 列表类

/*
 * 列表类
 * @brief 存储 [下标k, 双亲下标parent, 孩子链childs 和 元素data]的容器
 * 每个节点有三个属性 所处的下标k, 其双亲的下标parent 和 其孩子链
 * 可通过k来访问元素,修改元素
 * 也可通过元素值来推算其下标k
 * 一旦越界就报异常
 */

template <typename T>
class BiMap
{
public:
	int  put (T e, int parent = -1);			// 往里塞节点e 返回插入成功后e所在的下标
	void put (int k, T e, int parent = -1);		// 新建 下标为k 内容为e的节点
	void setParent (int k, int parent);			// 设置k处节点的parent域
	int getParent (int k);						// 获取k处节点的parent域
	auto& childs(int k)							// k处小伙子的孩子链表
	{return kMap[k].childs;}
	void setData(int k, const T& v);			// 设置k处节点的data
	T getData(int k);							// 获取k处节点的data
	int  find(const T& e);						// 寻找值为e的元素的下标
	void DeleteElem(int k);						// 删除k处的元素
	bool containLocation(int i);				// 下标i是否存在
	bool containElem(const T& elem);			// 所给元素是否存在

	size_t size();
	bool empty();

	//构造函数
	BiMap(initializer_list<int> parents			// 把parents和elems的东西塞进去 但是没有初始化每个节点的childs
		, initializer_list<T> elems);
	BiMap(vector<int> parents
		, vector<T> elems);
	BiMap() = default;							// 默认空构造函数


protected:
	int index = 0;
	unordered_map<int, BiMapNode<T> > kMap;
	unordered_map<T, int> vMap;
};


template<typename T>
inline int BiMap<T>::put(T e, int parent)
{
	put(index, e, parent);
	return index++;
}

template<typename T>
inline void BiMap<T>::put(int k, T e, int parent)
{
	BiMapNode<T> tmp(e, parent);
	kMap[k] = tmp;
	vMap[e] = k;
}

template<typename T>
inline void BiMap<T>::setParent(int k, int parent)
{
	kMap.at(k).parent = parent;
}

template<typename T>
inline int BiMap<T>::getParent(int k)
{
	return kMap.at(k).parent;
}

template<typename T>
inline void BiMap<T>::setData(int k, const T& v)
{
	auto tmp = &(kMap.at(k).data);
	vMap.erase(*tmp);
	vMap[v] = k;
	*tmp = v;
}

template<typename T>
inline T BiMap<T>::getData(int k)
{
	return kMap.at(k).data;
}

template<typename T>
inline int BiMap<T>::find(const T& e)
{
	return vMap.at(e);
}

template<typename T>
inline void BiMap<T>::DeleteElem(int k)
{
	auto tmp = &kMap.at(k).data; 	// 获取k对应的元素e
	vMap.erase(*tmp);				// 删除vMap中元素e对应的键值对
	kMap.erase(k);					// 删除kMap中下标k对应的键值对
}

template<typename T>
inline size_t BiMap<T>::size()
{
	return kMap.size();
}
template<typename T>
inline bool BiMap<T>::empty()
{
	return kMap.empty();
}
template<typename T>
BiMap<T>::BiMap(initializer_list<int> parents, initializer_list<T> elems)
{
	if(parents.size() != elems.size())
		throw std::length_error("数组长度不一致");

	auto pi = parents.begin(); auto ei = elems.begin();
	int size = parents.size();
	int i = 0;

	for(; i < size; i++, pi++, ei++)
		put(i, *ei, *pi);
	index = i;
}
template<typename T>
BiMap<T>::BiMap(vector<int> parents, vector<T> elems)
{
	if(parents.size() != elems.size())
		throw std::length_error("数组长度不一致");

	auto pi = parents.begin(); auto ei = elems.begin();
	int size = parents.size();
	int i = 0;

	for(; i < size; i++, pi++, ei++)
		put(i, *ei, *pi);
	index = i;
}
template<typename T>
inline bool BiMap<T>::containLocation(int i)
{
	return !(kMap.find(i) == kMap.end());
}
template<typename T>
inline bool BiMap<T>::containElem(const T& elem)
{
	return !(vMap.find(elem) == vMap.end());
}

template <typename T>
struct BiMapNode
{
	int parent = -1;
	T data;
	list<int> childs;

	BiMapNode(T data, int parent)
		: data(data), parent(parent)
	{}

	//BiMapNode() = default; //不知道为什么不加这个编译器就过不了

};

#endif //BIMAP_HPP

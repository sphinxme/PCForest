#ifndef PCFOREST_HPP
#define PCFOREST_HPP

#include "BiMap.hpp"
#include "Printer.h"

#include <initializer_list>
#include <deque>
#include <utility>
#include <sstream>
#include <vector>
#include <sstream>

using std::pair;
using std::list;
using std::initializer_list;
using std::deque;
using std::string;
using std::vector;
using point = Printer::point;


/*
struct point
{
	int x,y;
	string data;

	point(int x = 0, int y = 0):x(x), y(y){}
	point(int x, int y, string&& data):x(x), y(y), data(data){}
};
*/

template <typename T>
class PCForest
{
protected:
	BiMap<T>  nodelist;				// 所有节点的列表
	typedef list<int>::iterator listIter;
public:
	list<int> roots;				// 存储所有树的根节点下标 递归时从这里开始

//构造函数
	PCForest(initializer_list<int> parents,
			 initializer_list<T> elems);
	PCForest(vector<int> parents,
		     vector<T> elems);
	PCForest() = default;


//写改删查
	void create(const T& elem, int parent = -1); 	// 把给定元素放在parent所指元素的下面 默认为根节点
	void update(const T& before, const T& newElem);	// 把原来是before的第一个元素替换成T 没有的话报std::out_of_range异常
	T  retrieve(int k);								// 返回k处的元素的值
	void delete_(int k);							// 删除k处节点及其子节点(顶层删除)
	void deletee(int k);							// (递归删除)

	int Find(const T& elem);						// 返回所给值所在节点的下标 不存在的话返回-1
	void assign(int k, const T& elem);				// 更改给定位置的元素的值
	bool containLocation(int i);					// 返回所给下标是否存在
	bool containElem(const T& elem);				// 返回元素是否已经存在

//三种遍历
public:
	template<typename Predicate>										// 先序遍历
	void PreOrder (Predicate Visit);
	template<typename Predicate>										// 中序遍历
	void InOrder  (Predicate Visit);
	template<typename Predicate>										// 后序遍历
	void PostOrder(Predicate Visit);

	// 生成三种遍历的字符串
	string preStr (int x);
	string inStr  (int x);
	string postStr(int x);


protected:
	template<typename Predicate>
	void PreOrder (int x, Predicate Visit);
	template<typename Predicate>
	void InOrder  (int x, Predicate Visit);
	template<typename Predicate>
	void PostOrder(int x, listIter nextSibling, listIter end, Predicate Visit);


//管理树
public:

	size_t size() {return roots.size();}	//森林的规模(森林中树的数量)
	bool empty() {return roots.empty();}	//是否为空
	int height();							//森林的高度(森林中最大树的高度)
	int leaf();								//森林的叶子数

	int height(int k);						//求给定节点的树的高度(递归用)
	int leaf(int k);						//求给定节点的叶子数(递归用)

//显示输出相关
public:
	deque<point> Print(int n, point start);		// 生成输出roots中第n棵树的坐标序列(顶层非递归)
	point initPrint(int x, point& start, deque<point>& printList);	// 递归生成下标为x的节点为根的树
};

template<typename T>
PCForest<T>::PCForest(initializer_list<int> parents, initializer_list<T> elems)
:nodelist(parents, elems)
{
	// 如果传进来的parents跟elems不一样长的话那就抛异常=

	// parents跟elems的内容已经在冒号语法里传给nodelist了
	// 然后遍历nodelist 初始化每个节点的childs
	int size = parents.size();//控制循环终止
	int tmp_parent;//用于暂存遍历时每一个节点的parent域
	for(int i = 0; i < size; i++)//开始遍历nodelist中的每一个节点
	{
		tmp_parent = nodelist.getParent(i);
		if(tmp_parent < 0)
			roots.push_back(i);
		else
			nodelist.childs(tmp_parent).push_back(i);
	}
}

template<typename T>
PCForest<T>::PCForest(vector<int> parents, vector<T> elems)
:nodelist(parents, elems)
{
	int size = parents.size();//控制循环终止
	int tmp_parent;//用于暂存遍历时每一个节点的parent域
	for(int i = 0; i < size; i++)//开始遍历nodelist中的每一个节点
	{
		tmp_parent = nodelist.getParent(i);
		if(tmp_parent < 0)
			roots.push_back(i);
		else
			nodelist.childs(tmp_parent).push_back(i);
	}
}

template<typename T>
void PCForest<T>::create(const T& elem, int parent)
{
	// 加到节点集
	nodelist.put(elem, parent);

	// 认爹
	if(parent < 0)
		// 如果宁就是根节点 认爹认到roots里
		roots.push_back(nodelist.put(elem));	 // 把put所返回的值(也就是加入元素的下标)添加到roots里面
	else
		// 如果是谁的孩子 就给他爹后面记一笔
	{
		nodelist.childs(parent).push_back(nodelist.put(elem, parent));
		// TODO: 如果根节点的parent域要存储 -子节点个数的话 还要在以下部分添加:所属树根节点parent值-1 的代码
	}

}

template<typename T>
void PCForest<T>::update(const T& before, const T& newElem)
{
	int location = nodelist.find(before);
	nodelist.setData(location, newElem);
}

template<typename T>
inline T PCForest<T>::retrieve(int k)
{
	return nodelist.getData(k);
}

template<typename T>
void PCForest<T>::delete_(int k)
{
	int parent = nodelist.getParent(k);
	deletee(k);
	// 如果是个根节点 还要去roots那边清场
	if(parent < 0)
		roots.remove(k);
	else
		nodelist.childs(parent).remove(k);
}

template<typename T>
void PCForest<T>::deletee(int k)
{
	// 递归删除子节点
	for(auto &i : nodelist.childs(k))
		deletee(i);

	/* TODO: 如果根节点parent存储的是 -子节点数 的话
	 * 1 类多一个内部成员函数专门用来找自己的根节点
	 * 2 本函数分成两个 一个顶层函数不返回值 另一个是递归函数 返回值为int型 返回调用函数时删掉的元素数量
	 * 4 递归函数每次统计自己及下层递归删掉的元素数量
	 * 3 顶层函数调用递归函数 然后删根节点的parent域
	 */

	// 如果是个根节点 还要去roots那边清场
	//if(nodelist.getParent(k) < 0)
		//roots.remove(k);

	// 在nodelist里面删掉自己
	nodelist.DeleteElem(k);
}


template<typename T>
inline int PCForest<T>::Find(const T& elem)
{
	try
	{
		return nodelist.find(elem);
	}
	catch (std::out_of_range&)
	{
		return -1;
	}

}

template<typename T>
inline void PCForest<T>::assign(int k, const T& elem)
{
	// TODO: ...是不是该注意什么没注意到啊 这会儿忘记了
	nodelist.setData(k, elem);
}

template<typename T>
template<typename Predicate>
void PCForest<T>::PreOrder(Predicate Visit)
{
	//Visit(nodelist.getData(x));

	for(auto i : roots)
		PreOrder(i, Visit);
}

template<typename T>
template<typename Predicate>
void PCForest<T>::PreOrder(int x, Predicate Visit)
{
	Visit(nodelist.getData(x));

	for(auto i :nodelist.childs(x))
		PreOrder(i, Visit);
}

template<typename T>
template<typename Predicate>
void PCForest<T>::InOrder(Predicate Visit)
{
	for(auto i :roots)
		InOrder(i, Visit);

	//Visit(nodelist.getData(x));
}

template<typename T>
template<typename Predicate>
void PCForest<T>::InOrder(int x, Predicate Visit)
{
	for(auto i :nodelist.childs(x))
		PreOrder(i, Visit);

	Visit(nodelist.getData(x));
}

template<typename T>
template<typename Predicate>
void PCForest<T>::PostOrder(Predicate Visit)
{

//后根遍历第一棵树的根节点的子树森林
	auto child = roots.begin();
	auto childend = roots.end();
	PostOrder(*child++, child, childend, Visit);

//后根遍历除第一棵树之外其他树组成的森林
	// 虚拟根节点不存在下一个兄弟森林 故注释
	/* if(nextSibling != end)
		PostOrder(*nextSibling, ++nextSibling, end, Visit);*/

//访问森林中第一棵树的根节点
	//虚拟根节点不需要读取值
	//Visit(nodelist.getData(x));
}

template<typename T>
template<typename Predicate>
void PCForest<T>::PostOrder(int x, PCForest::listIter nextSibling, PCForest::listIter end, Predicate Visit)
{
	// 如果自己下面还有孩子的话
	if(!nodelist.childs(x).empty())
	{
	//后根遍历第一棵树的根节点的子树森林
		auto child = nodelist.childs(x).begin();        //找到子森林中 第一个树的根节点
		auto childend = nodelist.childs(x).end();        //算出来递归终点
		PostOrder(*child++, child, childend, Visit);    //让自己的孩子从child到childend递归执行
	}

	//后根遍历除第一棵树之外其他树组成的森林
	if(nextSibling != end)

		PostOrder(*nextSibling++, nextSibling, end, Visit);

	//访问森林中第一棵树的根节点
	Visit(nodelist.getData(x));
}

template<typename T>
int PCForest<T>::height()
{
	int heights = 0;
	int tmp;
	for(auto i : roots)
	{
		tmp = height(i);
		if(heights < tmp)
			heights = tmp;
	}
	return heights;	// 参见递归版本函数
}

template<typename T>
int PCForest<T>::height(int k)
{
	int heights = 0;	// 用于返回的height值
	int tmp = 0;	// 比较时我拿来暂存各个子树高度的变量

	for(auto i: nodelist.childs(k))// 遍历节点k的child链 如果为空不循环直接返回0
	{
		tmp = height(i);	// 看看各个子树都是多大
		if(heights < tmp)		// 跟手上的值比比 比手上的大的换掉
			heights = tmp;
	}

	return ++heights;			//带上自己这层高度
}

template<typename T>
int PCForest<T>::leaf()
{
	int leafs = 0;
	for(auto i : roots)
		leafs += leaf(i);

	return leafs;
}

template<typename T>
int PCForest<T>::leaf(int k)
{
	// 判断自己是不是叶子节点 是的话带上自己一个
	if(nodelist.childs(k).empty())
		return 1;

	// 不是的话向下递归
	int leafs = 0;// 用于返回叶子数的变量
	for(auto i : nodelist.childs(k))
		leafs += leaf(i);

	return leafs;
}
template<typename T>
point PCForest<T>::initPrint(int x, point& start, deque<point>& printList)
{
	point copy = start;
	point ret(start.x + 3, start.y);

	//后序遍历 有孩子的话先画孩子们
	if(!nodelist.childs(x).empty())
	{
		start.y += 6;
		deque<point> childLocation; // 记录每个子树的位置 用来画上面的引线
		for (auto i : nodelist.childs(x))
			childLocation.push_back(initPrint(i, start, printList));

		// 假设现在start已经迭代到右边第一个可以画的区域了

		// 画引线
		--start.y; // 上移一层画引线
		auto iter = nodelist.childs(x).begin();
		for (auto i : childLocation)
			printList.emplace_back(i.x, start.y, "|" + std::to_string(*iter++));

		// 画横线
		--start.y; //再上移一层
		string crossline;
		if(childLocation.size() == 1)
			crossline = '|';
		else
		{
			crossline = '+';
			crossline.append((childLocation.back().x - childLocation.front().x - 1)
					  , '-');
			crossline.push_back('+');
		}
		printList.emplace_back(childLocation.front().x, start.y, crossline);

		// 画自己下面那根线
		--start.y;
		ret.x = ( childLocation.front().x + childLocation.back().x ) / 2;
		printList.emplace_back(ret.x, start.y, "|");

		start.y = ret.y;
	}
	else
	{
		start.x += 8;
	}
	// 至此start跟ret都准备好了

	// 画框框跟元素
	printList.emplace_back(ret.x-3, ret.y  , "+-----+");
	printList.emplace_back(ret.x-3, ret.y+1, "|     |");
	printList.emplace_back(ret.x-3, ret.y+2, "+-----+");

	// 实在想不到其他的普适的类型T转string的方法了
	std::ostringstream os;
	os << nodelist.getData(x);
	string data = os.str();

	int pos = data.size() / 2;
	printList.emplace_back(ret.x-pos, ret.y+1, data);

	return ret;
}
template<typename T>
deque<point> PCForest<T>::Print(int n, point start)
{
	if(n >= roots.size())
		throw std::out_of_range("超出范围");

	auto iter = roots.begin();
	for(int i = 0; i < n; i++)
		iter++;

	deque<point> ret;
	initPrint(*iter, start, ret);

	ret.emplace_back(51, 2, std::to_string(*iter));

	return std::move(ret);
}
template<typename T>
string PCForest<T>::preStr(int x)
{
	// roots中的第x棵树 先迭代到这个根的地方
	auto iter = roots.begin();
	for(int i = 0; i < x; i++)
		iter++;
	int i = *iter;

	std::stringstream pre;
	PreOrder(i, [&](T e) {pre << e << " ";});
	return pre.str();
}

template<typename T>
string PCForest<T>::inStr(int x)
{
	auto iter = roots.begin();
	for(int i = 0; i < x; i++)
		iter++;
	int i = *iter;

	std::stringstream in;
	InOrder(i, [&](T e) {in << e << " ";});
	return in.str();
}
template<typename T>
string PCForest<T>::postStr(int x)
{
	auto iter = roots.begin();
	for(int i = 0; i < x; i++)
		iter++;
	int i = *iter;

	std::stringstream post;
	if(!nodelist.childs(i).empty())
		PostOrder(*nodelist.childs(i).begin(), ++nodelist.childs(i).begin(), nodelist.childs(i).end(), [&](T e) {post << e << " ";});
	post << nodelist.getData(i);
	return post.str();
}
template<typename T>
inline bool PCForest<T>::containLocation(int i)
{
	return nodelist.containLocation(i);
}
template<typename T>
inline bool PCForest<T>::containElem(const T& elem)
{
	return nodelist.containElem(elem);
}


#endif //PCFOREST_HPP

#ifndef TESTER_H
#define TESTER_H

#include "Printer.h"
#include "PCForest.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <list>
#include <utility>

using std::to_string;

class Tester
{
protected:
	Printer printer;				// 显示输出相关
	PCForest<char> forest;			// 数据
	std::list<int> &trees;

public:

	Tester(initializer_list<int> parents,
		   initializer_list<char> elems);

	Tester(vector<int>& parents,
		   vector<char>& elems);

	int treeView(int i);
	void exec();

	void test() { forest.create('i');  std::cout << forest.postStr(1) << std::endl;}

protected:
	void testvisit();

};


#endif //TESTER_H

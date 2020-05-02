
#include "PCForest.hpp"
#include "Printer.h"
#include "Tester.h"

#include <cstdlib>
#include <iostream>

using namespace std;

void input(Tester*& tester);
/*
int main()
{
	Tester me({-1, 0, 0, 0, 1, 1, 3, 3}
		,{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'});
	me.test();
	return 0;
}
*/

int main()
{
	system("chcp 65001 > nul");
	Tester* tester = nullptr;

	char c;
	cout << "是否使用默认测试样例(Y/n)" << endl;
	cin >> c;
	cin.get();
	if(c == 'N' || c == 'n')
		input(tester);
	else
		tester = new Tester({-1, 0, 0, 0, 1, 1, 3, 3}
						   ,{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'});

	tester->exec();
	//tester->test();

	cout << "程序执行结束 回车继续" << endl;
	getchar();
	getchar();

	if(!tester)
		delete tester;

	return 0;
}

void input(Tester*& tester)
{
	/*
	string parent,elem;
	cin >> parent;
	stringstream ssp(parent);
	*/
}

/*
int main()
{
	PCForest<char> t({-1, 0, 0, 0, 1, 1, 3, 3}
					,{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}
					);
	//t.print();
	cout << "height = " << t.height() << " size = " << t.size() << endl;
	//system("pause");

	Printer ppp;
	deque<point> d(t.Print(0, ppp.start));
	ppp.init();
	ppp.printTitle("第2棵树|共计7棵");
	ppp.printSide("# 树高: 66");
	ppp.printSide("# 先序遍历:\n\tA B C D E F G");
	ppp.printTree(t.Print(0, ppp.start));
	ppp.printBanner("最大高度11 叶子数12");
	//ppp.printSide("\tA B C D E F H I J K L");

	char s;
	cin >> s;

	std::cout << "Hello, World!" << std::endl;

	return 0;
}
*/
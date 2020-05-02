#ifndef PRINTER_H
#define PRINTER_H

#include <deque>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <utility>

/* VT100控制码
 * \033[0m		// 关闭所有属性
 * \033[1m		// 设置为高亮
 * \033[4m		// 下划线
 * \033[5m		// 闪烁
 * \033[7m		// 反显
 * \033[8m		// 消隐
 * \033[nA		// 光标上移 n 行
 * \033[nB		// 光标下移 n 行
 * \033[nC		// 光标右移 n 行
 * \033[nD		// 光标左移 n 行
 * \033[y;xH	// 设置光标位置
 * \033[2J		// 清屏
 * \033[K		// 清除从光标到行尾的内容
 * \033[s		// 保存光标位置
 * \033[u		// 恢复光标位置
 * \033[?25l	// 隐藏光标
 * \033[?25h	// 显示光标
 */

/*
 * 前景/背景 		色
 * 3/4 		0: 黑色
 * 3/4 		1: 红色
 * 3/4 		2: 绿色
 * 3/4 		3: 黄色
 * 3/4 		4: 蓝色
 * 3/4 		5: 紫色
 * 3/4 		6: 青色
 * 3/4 		7: 白色
 */

namespace Printers
{
	using namespace std;

	class Printer
	{
	public:
		struct point
		{
			int x,y;
			string data;

			point(int x = 0, int y = 0):x(x), y(y){}
			point(int x, int y, string data):x(x), y(y), data(std::move(data)){}
		};

		//窗口大小
		int cols = 150;
		int lines = 35;
		int sides = 50; //边栏大小
		//元素颜色
		int foreColor = 37;		//前景色
		int backColor = 41;		//背景色
		//界面整体颜色
		int background = 47;
		int banner = 44;
		int font = 37;
		int sider = 43;

		point start = point(50+5, 3);

	protected:
		bool reprint = true;


	public:
		void init();								// 大致加载
		void printTree(const deque<point>& list);	// 打印给定树传过来的序列
		void printSide(const string& str);			// 打印字符串到侧边栏
		void printTitle(const string& title);		// 打印字符串为标题
		void printBanner(const string& banner);		// 打印顶栏
		void input();								// 调整光标

	protected:
		void resize();								// 调整窗口大小
		void flush();								// 刷新窗口(清屏)
		void side();								// 刷新侧边栏

		void printLists(const deque<point>& list);
		void resume() const
		{ printf("\033[%d;%dm\033[0m\033[%d;%dH\033[1;1H", font, banner, lines-1, cols);}

	};

}



using Printers::Printer;




#endif //PRINTER_H

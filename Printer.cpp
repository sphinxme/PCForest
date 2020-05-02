
#include "Printer.h"
using namespace Printers;

void Printer::resize()
{
	//system("chcp 65001 > nul");
	string resize;

#ifdef _WIN32
	resize = "mode con cols=" + std::to_string(cols) + " lines=" + std::to_string(lines);
#else
	resize = "resize -s " + std::to_string(lines) + " " + std::to_string(cols);
#endif
	system(resize.c_str());
	printf("\033c");
	printf("\033c");

}
void Printer::flush()
{
	printf("\033c");
	string aLine(cols, ' ');

	//第一行
	//先把颜色设置成font banner 然后输出aLine 换行
	printf("\033[1;1H\033c");
	printf("\033[%d;%dm%s\n", font, banner, aLine.c_str());
	//printf("\033[1;1H");
	//printBanner();

	//剩下的行
	for(int i = 1; i < lines-2; i++)
		printf("\033[%d;%dm%s\n", font, background, aLine.c_str());

	//最后一行
	printf("\033[%d;%dm%s\n", font, banner, aLine.c_str());
	resume();
}

void Printer::printLists(const std::deque<point>& list)
{
	// 设置颜色
	printf("\033[%d;%dm", foreColor, backColor);

	//
	for(const auto& i : list)
		printf("\033[%d;%dH%s", i.y, i.x, i.data.c_str());

	resume();

}
void Printer::side()
{
	string sideline(sides, ' ');

	//printf("\033c");
	printf("\033[2;1H");

	//剩下的行
	for(int i = 1; i < lines-2; i++)
		printf("\033[%d;%dm%s\n", font, banner, sideline.c_str());

	resume();
}
void Printers::Printer::init()
{
	resize();
	flush();
	side();
	reprint = true;
}

void Printers::Printer::printTree(const deque<point>& list)
{
	printLists(list);
}
void Printers::Printer::printSide(const string& str)
{
	static int line = 7;
	if(reprint)
	{
		line = 7;
		reprint = false;
	}
	//指定样式 定位位置
	printf("\033[%d;%dm\033[%d;4H", font, banner, line);

	//切片输出 注意换行
	int col = 4;
	for(auto &i : str)
	{
		if(i != '\n')
			printf("%c", i);
		else
			printf("\033[%d;4H", ++line);

		if (col++ > 46)
		{
			col = 4;
			printf("\033[%d;4H", ++line);
		}
	}
	line += 2;
	resume();
}
void Printers::Printer::printTitle(const string& title)
{
	string border(21, '#');
	//指定样式 定位位置
	printf("\033[%d;%dm\033[3;15H", font, banner);
	printf("%s", border.c_str());
	printf("\033[5;15H%s", border.c_str());
	printf("\033[4;15H#  %s", title.c_str());
	printf("\033[4;35H#");
	resume();
}
void Printers::Printer::printBanner(const string& banner)
{
	int position = cols - banner.size();
	printf("\033[1;%dH\033[%d;%dm%s", position, font, this->banner, banner.c_str());
	resume();

}
void Printers::Printer::input()
{
	printf("\033[%d;1H\033[%d;%dm", lines-5, font, banner);
}



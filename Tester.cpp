
#include "Tester.h"

void Tester::exec()
{
	// 检测树是否为空
	if(forest.empty())
	{
		//如果为空直接输出是否添加新树
		std::cout << "当前森林为空(无子树根节点),请添加新树" << std::endl;
		std::cout << "请输入一个元素(char型):" << std::endl;
		char t;
		std::cin >> t;
		forest.create(t);
	}
	//不为空的话当我没说

	//先做一次的循环
	int status = 0;
	char input;
	do
	{
		treeView(status);
		//接收输入 u上一个/d下一个 | 1插入新树 2插入新节点 3删除节点 4删除当前树 5更改节点值 6遍历森林前/中/后 q返回
		std::cin >> input;
		switch (input)
		{
			case 'u'://上一个
				status = status > 0 ? --status : status;
				break;
			case 'd'://下一个
				status = (status + 1 < forest.size()) ? ++status : status;
				break;
			case '1'://插入新树
			{
				char elem;
				while(true)
				{
					std::cout << "请输入元素值(char型)" << std::endl;
					std::cin >> elem;
					if (!forest.containElem(elem))
						break;
					std::cout << "所输入元素与森林已有元素重复,请重新输入" << std::endl;
				}
				forest.create(elem);
			}
				break;
			case '2'://插入新节点
			{
				char elem;
				int parent;
				while(true)
				{
					std::cout << "请输入插入的节点位置(双亲下标)" << std::endl;
					std::cin >> parent;
					if(forest.containLocation(parent))
						break;
					else
						std::cout << "所输入下标不在森林中,请重新输入" << std::endl;
				}
				while(true)
				{
					std::cout << "请输入插入的元素值(char型)" << std::endl;
					std::cin >> elem;
					if(!forest.containElem(elem))
						break;
					std::cout << "所输入元素与森林已有元素重复,请重新输入" << std::endl;
				}
				forest.create(elem, parent);
			}
				break;
			case '3'://删除节点
			{
				std::cout << "请输入删除节点的位置(下标)" << std::endl;
				int in;
				while (true)
				{
					std::cin >> in;
					if(forest.containLocation(in))
						break;
					else
						std::cout << "所输入下标不在森林中,请重新输入" << std::endl;
				}
				int p_size = forest.size();
				forest.delete_(in);
				int n_size = forest.size();
				if(p_size != n_size) //检测是否有树发生改变
					status = n_size==0 ? -1 : 0;
			}
				break;
			case '4':// 删除当前树
			{
				auto iter = forest.roots.begin();
				for(int i=0; i < status; i++)
					iter++;
				forest.delete_(*iter);
				status = forest.empty() ? -1 : 0;
			}
				break;
			case '5':
			{
				std::cout << "输入要修改元素的下标" << std::endl;
				int in;
				while (true)
				{
					std::cin >> in;
					if(forest.containLocation(in))
						break;
					else
						std::cout << "所输入下标不在森林中,请重新输入" << std::endl;
				}
				char elem;
				while(true)
				{
					std::cout << "请输入插入的元素值(char型)" << std::endl;
					std::cin >> elem;
					if(!forest.containElem(elem))
						break;
					std::cout << "所输入元素与森林已有元素重复,请重新输入" << std::endl;
				}
				forest.assign(in, elem);
			}
				break;
			case '6'://三种遍历
				testvisit();
				break;
			case 'q'://退出
				status = -1;
				break;
			default:
				break;
		}
	}while (status != -1);

}
int Tester::treeView(int i)
{
	printer.init();
	printer.printBanner("森林最大高度: " + to_string(forest.height()) +
						" 叶子总数: " + to_string(forest.leaf()) );
	printer.printTitle("第" + to_string(i) + "棵树|共计" + to_string(forest.size()) + "棵" );
	printer.printTree(forest.Print(i, printer.start) );
	printer.printSide("#树叶子数: " + to_string(forest.leaf(i)) );

	printer.printSide("#先序遍历:\n\t" + forest.preStr(i));
	printer.printSide("#中序遍历:\n\t" + forest.inStr(i));
	printer.printSide("#后序遍历:\n\t" + forest.postStr(i));

	//接收输入 u上一个/d下一个 | 1插入新树 2插入新节点 3删除节点 4删除当前树 5更改节点值 6遍历森林前/中/后 q返回
	printer.printSide("[功能操作]\n[u]/[d] 上一个/下一个");
	printer.printSide("[1]/[2] 插入新树/新节点");
	printer.printSide("[3]/[4] 删除节点/当前树");
	printer.printSide("[5]     更改节点值");
	printer.printSide("[6]     遍历整个森林");
	printer.printSide("[q]     退出");

	printer.input();

	return 0;
}
Tester::Tester(initializer_list<int> parents, initializer_list<char> elems)
:forest(parents, elems), trees(forest.roots)
{}
Tester::Tester(vector<int>& parents, vector<char>& elems)
:forest(std::move(parents), std::move(elems)), trees(forest.roots)
{}
void Tester::testvisit()
{
	std::cout << "选择遍历方式[1]先序[2]中序[3]后序" << std::endl;
	int in;

	std::cin >> in;
	getchar();
	switch (in)
	{
	case 1:
		forest.PreOrder([](char e)
		{ std::cout << e; });
		getchar();
		break;
	case 2:
		forest.InOrder([](char e)
		{ std::cout << e; });
		getchar();
		break;
	case 3:
		forest.PostOrder([](char e)
		{ std::cout << e; });
		getchar();
		break;
	default:
		std::cout << "不对不对" << std::endl;
		getchar();
		break;
	}
}

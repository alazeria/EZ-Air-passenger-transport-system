// 航空客运系统.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "functions.h"


int main()
{
	L = get_airline();
	do
	{
		int f = 1;
		switch (login())
		{
		case 1:/*乘客界面*/
			system("pause");
			system("cls");
			do
			{
				switch (form1())
				{
				case 1:/*调用查询函数*/
					system("cls");
					search();
					system("pause");
					system("cls");
					break;
				case 2:/*订票*/
					system("cls");
					booking();
					system("pause");
					system("cls");
					break;
				case 3:/*退票*/
					system("cls");
					refund();
					system("pause");
					system("cls");
					break;
				case 0:/*退出登录，返回上一层*/
					f = 0;
					system("cls");
				}
			} while (f);
			break;
		case 2:/*管理员界面*/
			system("pause");
			system("cls");
			do
			{
				switch (form2())
				{
				case 1:/*查看所有航班信息*/
					system("cls");
					print_airline();
					system("pause");
					system("cls");
					break;
				case 2:/*查看乘客名单*/
					system("cls");
					client_list();
					system("pause");
					system("cls");
					break;
				case 3:/*修改票价*/
					system("cls");
					re_price();
					system("pause");
					system("cls");
					break;
				case 0:/*退出登录，返回上一层*/
					f = 0;
					system("cls");
				}
			} while (f);
		}

	} while (1);
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

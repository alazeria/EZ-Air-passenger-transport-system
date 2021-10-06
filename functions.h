#pragma once

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<cstring>
using namespace std;

int hello_world() {
	cout << "Hello World!\n";
	return 0;
}

struct client
{ /*客户*/
	char name[20];          /*姓名*/
	int ordering_number;    /*订票量*/
	int grade;             /*舱位等级*/
	client* next;
};

class airline
{ /*航班信息*/
private:
	char end_station[20];   /*终点站名*/
	char air_num[10];       /*航班号*/
	char plane_num[10];     /*飞机号*/
	char day[7];            /*飞行日（星期几）*/
	int people_quota;       /*乘员定额*/
	double price;           /*票价*/
	int more_ticket;        /*余票量*/
	char passenger_list[50];   /*乘客名单*/
	char waiting_list[50];     /*等候替补的客户名单*/
	static int number;       /*航班总数*/
public:
	void print()
	{/*打印航班信息*/
		cout << "  终点站名：" << end_station << "  \t 航班号:" << air_num << "\t    飞机号:" << plane_num
			<< "\n  时间:" << day << "\t\t  乘员定额:" << people_quota << "\n  票价:" << price << "\t\t 余票量:" << more_ticket << "\n\n";
	}
	friend airline* get_airline();                   /*读取文件中的航班信息*/
	friend void client_list();
	friend void put_airline(airline*);
	friend void print_airline();
	friend void search();
	friend void booking();
	friend void refund();
	friend client* get_passenger(airline);
	friend client* get_wait(airline);
	friend void put_passenger(airline, client*);
	friend void put_wait(airline, client*);
	friend void re_price();
};

int airline::number = 0;
char Name[20] = "";            /*用于用户登录的姓名*/
airline* L;                  /*全局变量，航线指针*/

void app_client(client* c, client* q)
{/*增加乘客*/
	client* p = c;
	while (p->next) p = p->next;
	p->next = q;
	q->next = NULL;
}

int login()
{/*登录*/
	int f;
	char password[20], * p = (char*)"666666";       /*管理员密码*/
	cout << "登录 >>>>>>>>>\n\n";
	cout << "           1.乘客登录         2.管理员登录\n  请选择用户身份：";
	fflush(stdin);   /*刷新输入流缓冲区*/
	while (f = int(getchar()) - 48)
	{
		if (f == 1)
		{
			cout << "  姓名：";
			cin >> Name;
			cout << "\n\t\t登录成功！！\n\n >>>>>>>>>\n\n";
			return 1;
		}
		else if (f == 2)
		{
			cout << "\n  输入管理员密码(提示6个6)：";
			fflush(stdin);
			while (cin >> password)
			{
				if (!strcmp(password, p))
				{
					cout << "\n\t\t登录成功！！\n\n >>>>>>>>>\n\n";
					return 2;
				}
				else cout << "\n密码错误！！ 请重新输入\n\n  输入管理员密码：";
			}
		}
		else cout << "输入错误！！ 请重新输入\n  请选择用户身份： ";
	}
	return 0;
}

void search()
{/*查询操作*/
	char s[20];
	int i = 0;
	cout << "查询 >>>>>>>>>\n\n\t\t终点站名:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].end_station)) i++;    /*按链表顺序匹配终点站名*/
	if (i < L->number)
		L[i].print();
	else cout << "\n无此航班信息!\n\n";
	cout << " >>>>>>>>>\n\n";
}

void refund()
{/*退票操作*/
	char s[20];
	int i = 0, n;
	cout << "退票 >>>>>>>>>\n\n\t\t航班号:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;       /*按航班号匹配*/
	if (i < L->number)
	{/*找到此航班*/
		client* c1 = get_passenger(L[i]), * c2 = get_wait(L[i]), * p = c1, * q, * r;
		/*获得次航班的乘客和候补乘客链表*/

		while (p->next && strcmp(Name, p->next->name)) p = p->next;     /*姓名查找*/
		if (p->next)
		{/*找到该用户姓名*/
			cout << "您在" << L[i].air_num << "航班(终点站：" << L[i].end_station << ")订了"
				<< p->next->ordering_number << "张票\n请输入退票数量：";
			do
			{/*输入退票数*/
				cin >> n;
				if (n > 0 && n <= p->next->ordering_number) break;
				else cout << "\n输入错误！！请重新输入：";
			} while (1);
			if (n == p->next->ordering_number)
			{/*全部退票*/
				r = p->next;
				q = r->next;
				p->next = q;
				delete r;
			}
			else if (n < p->next->ordering_number) p->next->ordering_number -= n;  /*退部分票*/
			L[i].more_ticket += n;   /*余票增加*/
			int f = 1;
			while (L[i].more_ticket && f)
			{/*有余票且未访问完所有候补乘客*/
				p = c2;
				while (p->next && L[i].more_ticket < p->next->ordering_number) p = p->next;
				if (p->next)
				{/*找到需求票数满足的*/
					L[i].more_ticket -= p->next->ordering_number;
					r = p->next;
					q = r->next;
					p->next = q;
					app_client(c1, r);
				}
				else f = 0;  /*访问完*/
			}
			put_passenger(L[i], c1);   /*刷新乘客名单*/
			put_wait(L[i], c2);
		}
		else cout << "\n退票错误！！ 您没有在此航班订票\n";
	}
	else cout << "\n无此航班信息!\n\n";
	cout << "\n 退票完成！\n";
	cout << " >>>>>>>>>\n\n";
	put_airline(L);
}

void booking()
{/*订票操作*/
	char s[20];
	int i = 0, n, y, g;
	cout << "订票 >>>>>>>>>\n\n\t\t航班号:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;
	if (i < L->number)
	{/*找到该航班*/
		L[i].print();
		client* c1 = get_passenger(L[i]);
		client* c2 = get_wait(L[i]);
		cout << " 舱位等级（1：头等舱，2：普通舱，3：经济舱）："; cin >> g;
		cout << " 购买票数：";
		do
		{/*合理输入购买票数*/
			cin >> n;
			if (n <= L[i].people_quota) break;
			else
				cout << "错误！！不能超过最大乘员数（" << L[i].people_quota << "）,重新输入：";
		} while (1);

		if (L[i].more_ticket)
		{/*有余票*/
			if (n <= L[i].more_ticket)
			{/*余票足够*/
				client* p = new client;
				strcpy_s(p->name, Name);
				p->ordering_number = n;
				L[i].more_ticket -= n;
				p->grade = g;

				app_client(c1, p);
				cout << "\n 购票完成！\n";
			}
			else if (n > L[i].more_ticket)
			{/*余票小于购买数*/
				int x;
				client* p1 = new client, * p2 = new client;

				/*询问用户*/
				cout << "\n\n（1：只买" << L[i].more_ticket << "张  2：买" << L[i].more_ticket << "张，"
					<< n - L[i].more_ticket << "张等候  3：不买)\n 余票不足" << n << "张，是否继续购买？:";
				cin >> x;
				switch (x)
				{
				case 1:
					strcpy_s(p1->name, Name);
					p1->ordering_number = L[i].more_ticket;
					L[i].more_ticket = 0;
					p1->grade = g;
					app_client(c1, p1);
					cout << "\n 购票完成！\n";
					break;
				case 2:
					strcpy_s(p1->name, Name);
					p1->ordering_number = L[i].more_ticket;
					p1->grade = g;
					app_client(c1, p1);

					strcpy_s(p2->name, Name);
					p2->ordering_number = n - L[i].more_ticket;
					L[i].more_ticket = 0;
					p2->grade = g;
					app_client(c2, p2);

					cout << "\n 购票完成！\n";
					break;
				}
			}
		}
		else
		{/*无余票*/
			cout << "\n 无余票！ 是否排队等候票？（1：是 0：否）"; cin >> y;
			if (y)
			{/*进入等候名单*/
				client* p = new client;
				strcpy_s(p->name, Name);
				p->ordering_number = n;
				p->grade = g;
				app_client(c2, p);
			}
		}
		put_passenger(L[i], c1);    /*刷新乘客名单*/
		put_wait(L[i], c2);
	}
	else cout << "\n无此航班信息!\n\n";
	cout << "\n >>>>>>>>>\n\n";
	put_airline(L);
}

void put_wait(airline l, client* c)
{/*输出等候替补名单*/
	fstream f;
	client* p = c->next;
	f.open(l.waiting_list, ios::out | ios::trunc);
	while (p)
	{
		f << p->name << " " << p->ordering_number << " " << p->grade << '\n';
		p = p->next;
	}
	f.close();
}

client* get_passenger(airline l)
{/*读取某航班乘客信息*/
	fstream f; int i = 0;
	client* c = new client, * p = new client, * q = c;
	f.open(l.passenger_list, ios::in);
	while (f >> p->name >> p->ordering_number >> p->grade)
	{
		q->next = p;
		p = new client;
		q = q->next;
	}
	q->next = NULL;
	f.close();
	return c;
}

void put_passenger(airline l, client* c)
{/*输出乘客信息到文件*/
	fstream f;
	client* p = c->next;
	f.open(l.passenger_list, ios::out | ios::trunc);
	while (p)
	{
		f << p->name << " " << p->ordering_number << " " << p->grade << '\n';
		p = p->next;
	}
	f.close();
}

client* get_wait(airline l)
{/*读取某航班等候替补名单*/
	fstream f; int i = 0;
	client* c = new client, * p = new client, * q = c;
	f.open(l.waiting_list, ios::in);
	while (f >> p->name >> p->ordering_number >> p->grade)
	{
		q->next = p;
		p = new client;
		q = q->next;
	}
	q->next = NULL;
	f.close();
	return c;
}

airline* get_airline()
{/*读取文件中的航班信息*/
	fstream f; int i = 0;
	airline* line = new airline[100];
	f.open("航班信息.txt", ios::in);
	while (f >> line[i].end_station >> line[i].air_num >> line[i].plane_num >>
		line[i].day >> line[i].people_quota >> line[i].price >> line[i].more_ticket
		>> line[i].passenger_list >> line[i].waiting_list)
		i++;
	line->number = i;
	f.close();
	return line;
}

void put_airline(airline* L)
{/*输出修改后的航班信息*/
	fstream f; int i;
	f.open("航班信息.txt", ios::out);

	for (i = 0; i < L->number; i++)  f << L[i].end_station << " " << L[i].air_num << " "
		<< L[i].plane_num << " " << L[i].day << " " << L[i].people_quota << " " <<
		L[i].price << " " << L[i].more_ticket << " " << L[i].passenger_list << " " << L[i].waiting_list << '\n';
	f.close();
}

void print_airline()
{/*打印所有航班信息（管理员操作）*/
	L = get_airline();
	int i = 0;
	cout << "*航班信息 >>>>>>>>>\n\n";
	for (int i = 0; i < L->number; i++)
		L[i].print();
	cout << "\n >>>>>>>>>\n\n";
}

void client_list()
{/*查看某航班乘客名单（管理员操作）*/
	int i = 0; char s[20], * g = s;
	cout << "乘客名单 >>>>>>>>>*\n\n\t\t航班号:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;
	if (i < L->number)
	{
		client c;
		fstream f;
		f.open(L[i].passenger_list, ios::in);
		cout << "\n  已购票乘客名单：\n";
		while (f >> c.name >> c.ordering_number >> c.grade)
		{
			switch (c.grade)
			{
			case 1:g = (char*)"头等舱"; break;
			case 2:g = (char*)"普通舱"; break;
			case 3:g = (char*)"经济舱";
			}
			cout << "\t姓名：" << c.name << "\t舱位等级：" << g << "\t购票数：" << c.ordering_number << endl;
		}
		f.close();
		f.open(L[i].waiting_list, ios::in);
		cout << "\n  候补订票乘客名单：\n";
		while (f >> c.name >> c.ordering_number >> c.grade)
		{
			switch (c.grade)
			{
			case 1:g = (char*)"头等舱"; break;
			case 2:g = (char*)"普通舱"; break;
			case 3:g = (char*)"经济舱";
			}
			cout << "\t姓名：" << c.name << "\t舱位等级：" << g << "\t预订票数：" << c.ordering_number << endl;
		}
		f.close();
	}
	else cout << "\n无此航班信息!\n\n";
	cout << "\n >>>>>>>>>\n\n";
}

void re_price()
{/*修改某航班票价（管理员操作）*/
	int i = 0, p; char s[20];
	cout << "修改票价 >>>>>>>>>\n\n\t\t航班号:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;
	if (i < L->number)
	{
		cout << "  终点站名：" << L[i].end_station << "\t  飞机号:" << L[i].plane_num
			<< "\n\n\t    原票价：" << L[i].price << "\n\t    新票价：";
		cin >> p;
		L[i].price = p;
		cout << "\n   修改票价完成\n";
	}
	else cout << "\n无此航班信息!\n\n";
	put_airline(L);
	cout << "\n >>>>>>>>>\n\n";
}

int form1()
{/*乘客用户界面*/
	int s;
	cout << "用户界面 >>>>>>>>>\n\n";
	cout << "   1.查询航班\t   2.订票\t  3.退票     0.退出登录\n\n\t\t选择操作：";
	fflush(stdin);
	while (s = int(getchar()) - 48)
	{
		if (s >= 0 && s <= 3)
		{
			cout << "\n >>>>>>>>>\n\n";
			return s;
		}
		else cout << "\t输入错误！！ \n\t请重新选择操作：";
	}
	return 0;
}

int form2()
{/*管理员界面*/
	int s;
	cout << "管理员界面 >>>>>>>>>\n\n";
	cout << "   1.航班信息\t   2.乘客名单\t  3.修改票价     0.退出登录\n\n\t\t选择操作：";
	fflush(stdin);
	while (s = int(getchar()) - 48)
	{
		if (s >= 0 && s <= 3)
		{
			cout << "\n >>>>>>>>>\n\n";
			return s;
		}
		else cout << "\t输入错误！！ \n\t请重新选择操作：";
	}
	return 0;
}

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
{ /*�ͻ�*/
	char name[20];          /*����*/
	int ordering_number;    /*��Ʊ��*/
	int grade;             /*��λ�ȼ�*/
	client* next;
};

class airline
{ /*������Ϣ*/
private:
	char end_station[20];   /*�յ�վ��*/
	char air_num[10];       /*�����*/
	char plane_num[10];     /*�ɻ���*/
	char day[7];            /*�����գ����ڼ���*/
	int people_quota;       /*��Ա����*/
	double price;           /*Ʊ��*/
	int more_ticket;        /*��Ʊ��*/
	char passenger_list[50];   /*�˿�����*/
	char waiting_list[50];     /*�Ⱥ��油�Ŀͻ�����*/
	static int number;       /*��������*/
public:
	void print()
	{/*��ӡ������Ϣ*/
		cout << "  �յ�վ����" << end_station << "  \t �����:" << air_num << "\t    �ɻ���:" << plane_num
			<< "\n  ʱ��:" << day << "\t\t  ��Ա����:" << people_quota << "\n  Ʊ��:" << price << "\t\t ��Ʊ��:" << more_ticket << "\n\n";
	}
	friend airline* get_airline();                   /*��ȡ�ļ��еĺ�����Ϣ*/
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
char Name[20] = "";            /*�����û���¼������*/
airline* L;                  /*ȫ�ֱ���������ָ��*/

void app_client(client* c, client* q)
{/*���ӳ˿�*/
	client* p = c;
	while (p->next) p = p->next;
	p->next = q;
	q->next = NULL;
}

int login()
{/*��¼*/
	int f;
	char password[20], * p = (char*)"666666";       /*����Ա����*/
	cout << "��¼ >>>>>>>>>\n\n";
	cout << "           1.�˿͵�¼         2.����Ա��¼\n  ��ѡ���û���ݣ�";
	fflush(stdin);   /*ˢ��������������*/
	while (f = int(getchar()) - 48)
	{
		if (f == 1)
		{
			cout << "  ������";
			cin >> Name;
			cout << "\n\t\t��¼�ɹ�����\n\n >>>>>>>>>\n\n";
			return 1;
		}
		else if (f == 2)
		{
			cout << "\n  �������Ա����(��ʾ6��6)��";
			fflush(stdin);
			while (cin >> password)
			{
				if (!strcmp(password, p))
				{
					cout << "\n\t\t��¼�ɹ�����\n\n >>>>>>>>>\n\n";
					return 2;
				}
				else cout << "\n������󣡣� ����������\n\n  �������Ա���룺";
			}
		}
		else cout << "������󣡣� ����������\n  ��ѡ���û���ݣ� ";
	}
	return 0;
}

void search()
{/*��ѯ����*/
	char s[20];
	int i = 0;
	cout << "��ѯ >>>>>>>>>\n\n\t\t�յ�վ��:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].end_station)) i++;    /*������˳��ƥ���յ�վ��*/
	if (i < L->number)
		L[i].print();
	else cout << "\n�޴˺�����Ϣ!\n\n";
	cout << " >>>>>>>>>\n\n";
}

void refund()
{/*��Ʊ����*/
	char s[20];
	int i = 0, n;
	cout << "��Ʊ >>>>>>>>>\n\n\t\t�����:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;       /*�������ƥ��*/
	if (i < L->number)
	{/*�ҵ��˺���*/
		client* c1 = get_passenger(L[i]), * c2 = get_wait(L[i]), * p = c1, * q, * r;
		/*��ôκ���ĳ˿ͺͺ򲹳˿�����*/

		while (p->next && strcmp(Name, p->next->name)) p = p->next;     /*��������*/
		if (p->next)
		{/*�ҵ����û�����*/
			cout << "����" << L[i].air_num << "����(�յ�վ��" << L[i].end_station << ")����"
				<< p->next->ordering_number << "��Ʊ\n��������Ʊ������";
			do
			{/*������Ʊ��*/
				cin >> n;
				if (n > 0 && n <= p->next->ordering_number) break;
				else cout << "\n������󣡣����������룺";
			} while (1);
			if (n == p->next->ordering_number)
			{/*ȫ����Ʊ*/
				r = p->next;
				q = r->next;
				p->next = q;
				delete r;
			}
			else if (n < p->next->ordering_number) p->next->ordering_number -= n;  /*�˲���Ʊ*/
			L[i].more_ticket += n;   /*��Ʊ����*/
			int f = 1;
			while (L[i].more_ticket && f)
			{/*����Ʊ��δ���������к򲹳˿�*/
				p = c2;
				while (p->next && L[i].more_ticket < p->next->ordering_number) p = p->next;
				if (p->next)
				{/*�ҵ�����Ʊ�������*/
					L[i].more_ticket -= p->next->ordering_number;
					r = p->next;
					q = r->next;
					p->next = q;
					app_client(c1, r);
				}
				else f = 0;  /*������*/
			}
			put_passenger(L[i], c1);   /*ˢ�³˿�����*/
			put_wait(L[i], c2);
		}
		else cout << "\n��Ʊ���󣡣� ��û���ڴ˺��ඩƱ\n";
	}
	else cout << "\n�޴˺�����Ϣ!\n\n";
	cout << "\n ��Ʊ��ɣ�\n";
	cout << " >>>>>>>>>\n\n";
	put_airline(L);
}

void booking()
{/*��Ʊ����*/
	char s[20];
	int i = 0, n, y, g;
	cout << "��Ʊ >>>>>>>>>\n\n\t\t�����:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;
	if (i < L->number)
	{/*�ҵ��ú���*/
		L[i].print();
		client* c1 = get_passenger(L[i]);
		client* c2 = get_wait(L[i]);
		cout << " ��λ�ȼ���1��ͷ�Ȳգ�2����ͨ�գ�3�����òգ���"; cin >> g;
		cout << " ����Ʊ����";
		do
		{/*�������빺��Ʊ��*/
			cin >> n;
			if (n <= L[i].people_quota) break;
			else
				cout << "���󣡣����ܳ�������Ա����" << L[i].people_quota << "��,�������룺";
		} while (1);

		if (L[i].more_ticket)
		{/*����Ʊ*/
			if (n <= L[i].more_ticket)
			{/*��Ʊ�㹻*/
				client* p = new client;
				strcpy_s(p->name, Name);
				p->ordering_number = n;
				L[i].more_ticket -= n;
				p->grade = g;

				app_client(c1, p);
				cout << "\n ��Ʊ��ɣ�\n";
			}
			else if (n > L[i].more_ticket)
			{/*��ƱС�ڹ�����*/
				int x;
				client* p1 = new client, * p2 = new client;

				/*ѯ���û�*/
				cout << "\n\n��1��ֻ��" << L[i].more_ticket << "��  2����" << L[i].more_ticket << "�ţ�"
					<< n - L[i].more_ticket << "�ŵȺ�  3������)\n ��Ʊ����" << n << "�ţ��Ƿ��������:";
				cin >> x;
				switch (x)
				{
				case 1:
					strcpy_s(p1->name, Name);
					p1->ordering_number = L[i].more_ticket;
					L[i].more_ticket = 0;
					p1->grade = g;
					app_client(c1, p1);
					cout << "\n ��Ʊ��ɣ�\n";
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

					cout << "\n ��Ʊ��ɣ�\n";
					break;
				}
			}
		}
		else
		{/*����Ʊ*/
			cout << "\n ����Ʊ�� �Ƿ��ŶӵȺ�Ʊ����1���� 0����"; cin >> y;
			if (y)
			{/*����Ⱥ�����*/
				client* p = new client;
				strcpy_s(p->name, Name);
				p->ordering_number = n;
				p->grade = g;
				app_client(c2, p);
			}
		}
		put_passenger(L[i], c1);    /*ˢ�³˿�����*/
		put_wait(L[i], c2);
	}
	else cout << "\n�޴˺�����Ϣ!\n\n";
	cout << "\n >>>>>>>>>\n\n";
	put_airline(L);
}

void put_wait(airline l, client* c)
{/*����Ⱥ��油����*/
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
{/*��ȡĳ����˿���Ϣ*/
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
{/*����˿���Ϣ���ļ�*/
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
{/*��ȡĳ����Ⱥ��油����*/
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
{/*��ȡ�ļ��еĺ�����Ϣ*/
	fstream f; int i = 0;
	airline* line = new airline[100];
	f.open("������Ϣ.txt", ios::in);
	while (f >> line[i].end_station >> line[i].air_num >> line[i].plane_num >>
		line[i].day >> line[i].people_quota >> line[i].price >> line[i].more_ticket
		>> line[i].passenger_list >> line[i].waiting_list)
		i++;
	line->number = i;
	f.close();
	return line;
}

void put_airline(airline* L)
{/*����޸ĺ�ĺ�����Ϣ*/
	fstream f; int i;
	f.open("������Ϣ.txt", ios::out);

	for (i = 0; i < L->number; i++)  f << L[i].end_station << " " << L[i].air_num << " "
		<< L[i].plane_num << " " << L[i].day << " " << L[i].people_quota << " " <<
		L[i].price << " " << L[i].more_ticket << " " << L[i].passenger_list << " " << L[i].waiting_list << '\n';
	f.close();
}

void print_airline()
{/*��ӡ���к�����Ϣ������Ա������*/
	L = get_airline();
	int i = 0;
	cout << "*������Ϣ >>>>>>>>>\n\n";
	for (int i = 0; i < L->number; i++)
		L[i].print();
	cout << "\n >>>>>>>>>\n\n";
}

void client_list()
{/*�鿴ĳ����˿�����������Ա������*/
	int i = 0; char s[20], * g = s;
	cout << "�˿����� >>>>>>>>>*\n\n\t\t�����:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;
	if (i < L->number)
	{
		client c;
		fstream f;
		f.open(L[i].passenger_list, ios::in);
		cout << "\n  �ѹ�Ʊ�˿�������\n";
		while (f >> c.name >> c.ordering_number >> c.grade)
		{
			switch (c.grade)
			{
			case 1:g = (char*)"ͷ�Ȳ�"; break;
			case 2:g = (char*)"��ͨ��"; break;
			case 3:g = (char*)"���ò�";
			}
			cout << "\t������" << c.name << "\t��λ�ȼ���" << g << "\t��Ʊ����" << c.ordering_number << endl;
		}
		f.close();
		f.open(L[i].waiting_list, ios::in);
		cout << "\n  �򲹶�Ʊ�˿�������\n";
		while (f >> c.name >> c.ordering_number >> c.grade)
		{
			switch (c.grade)
			{
			case 1:g = (char*)"ͷ�Ȳ�"; break;
			case 2:g = (char*)"��ͨ��"; break;
			case 3:g = (char*)"���ò�";
			}
			cout << "\t������" << c.name << "\t��λ�ȼ���" << g << "\tԤ��Ʊ����" << c.ordering_number << endl;
		}
		f.close();
	}
	else cout << "\n�޴˺�����Ϣ!\n\n";
	cout << "\n >>>>>>>>>\n\n";
}

void re_price()
{/*�޸�ĳ����Ʊ�ۣ�����Ա������*/
	int i = 0, p; char s[20];
	cout << "�޸�Ʊ�� >>>>>>>>>\n\n\t\t�����:";
	cin >> s;
	while (i < L->number && strcmp(s, L[i].air_num)) i++;
	if (i < L->number)
	{
		cout << "  �յ�վ����" << L[i].end_station << "\t  �ɻ���:" << L[i].plane_num
			<< "\n\n\t    ԭƱ�ۣ�" << L[i].price << "\n\t    ��Ʊ�ۣ�";
		cin >> p;
		L[i].price = p;
		cout << "\n   �޸�Ʊ�����\n";
	}
	else cout << "\n�޴˺�����Ϣ!\n\n";
	put_airline(L);
	cout << "\n >>>>>>>>>\n\n";
}

int form1()
{/*�˿��û�����*/
	int s;
	cout << "�û����� >>>>>>>>>\n\n";
	cout << "   1.��ѯ����\t   2.��Ʊ\t  3.��Ʊ     0.�˳���¼\n\n\t\tѡ�������";
	fflush(stdin);
	while (s = int(getchar()) - 48)
	{
		if (s >= 0 && s <= 3)
		{
			cout << "\n >>>>>>>>>\n\n";
			return s;
		}
		else cout << "\t������󣡣� \n\t������ѡ�������";
	}
	return 0;
}

int form2()
{/*����Ա����*/
	int s;
	cout << "����Ա���� >>>>>>>>>\n\n";
	cout << "   1.������Ϣ\t   2.�˿�����\t  3.�޸�Ʊ��     0.�˳���¼\n\n\t\tѡ�������";
	fflush(stdin);
	while (s = int(getchar()) - 48)
	{
		if (s >= 0 && s <= 3)
		{
			cout << "\n >>>>>>>>>\n\n";
			return s;
		}
		else cout << "\t������󣡣� \n\t������ѡ�������";
	}
	return 0;
}

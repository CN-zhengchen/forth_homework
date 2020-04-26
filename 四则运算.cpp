#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include <fstream>
#include<algorithm>
#include <sstream>
#include<random>
#include<ctime>
using namespace std;
int Tolsize = 0;//��Ŀ����
int sym[4] = { 0 };//�洢����ѡ��
int Max = 0;//�洢���ֵ
int judge[2] = { 0 };//�洢���ź�С���ж�
int way;//�����ʽ
int flag = 0;//�ж���Ŀ������
vector < vector<string> > save;
vector <float> Ans;//�洢��
vector <int> Error;//�洢�������
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
float S_cal(string num1, string s, string num2)
{
	float switch1 = stringToNum<float>(num1);
	float switch2 = stringToNum<float>(num2);
	int rep1;
	int rep2;
	if (s == "+")
	{
		switch1 += switch2;
	}
	else if (s == "-")
	{
		switch1 -= switch2;
	}
	else if (s == "*")
	{
		switch1 *= switch2;
	}
	else if (s == "/")
	{
		if (switch2 == 0)
		{
			flag = 1;
		}
		else if(judge[1]==0)
		{
			rep1 = switch1;
			rep2 = switch2;
			rep1 /= rep2;
			switch1 = rep1;
		}
		else
		{
			switch1 /= switch2;
		}
	}
	return switch1;
}
void calculate()
{
	float ans = 0;
	vector<string>::iterator  position;//���ҷ��ŵ�λ��
	vector<string>::iterator  position1;//������һ���ŵ�λ��
	for (int i = 0; i < save.size(); i++)
	{
		vector<string> equation;
		equation = save[i];
		position = find(equation.begin(), equation.end(), "(");
		if (position != equation.end())
		{
			ans = S_cal(*(position + 1), *(position + 2), *(position + 3));
			if (flag == 1)
			{
				Error.push_back(i);
				Ans.push_back(ans);
				flag = 0;
				continue;
			}
			equation.insert(position, to_string(ans));
			position = find(equation.begin(), equation.end(), "(");
			equation.erase(position, position + 5);
		}
		while (true)
		{
			position = find(equation.begin(), equation.end(), "*");
			position1 = find(equation.begin(), equation.end(), "/");
			if (position == position1 && position == equation.end())
			{
				break;
			}
			else if (position<=position1)
			{
				ans = S_cal(*(position - 1), *(position), *(position + 1));
				equation.insert(position - 1, to_string(ans));
				position = find(equation.begin(), equation.end(), "*");
				equation.erase(position - 1, position + 2);
			}
			else
			{
				ans = S_cal(*(position1 - 1), *(position1), *(position1 + 1));
				if (flag == 1)
				{
					Error.push_back(i);
					break;
				}
				equation.insert(position1 - 1, to_string(ans));
				position1 = find(equation.begin(), equation.end(), "/");
				equation.erase(position1 - 1, position1 + 2);
				if (flag == 1)
				{
					break;
				}
			}
		}
		if (flag == 1)
		{
			flag = 0;
			Ans.push_back(ans);
			continue;
		}
		while (1)
		{
			position = find(equation.begin(), equation.end(), "-");
			if (position != equation.end())
			{
				ans = S_cal(*(position - 1), *(position), *(position + 1));
				equation.insert(position - 1, to_string(ans));
				position = find(equation.begin(), equation.end(), "-");
				equation.erase(position - 1, position + 2);
			}
			else
			{
				break;
			}
		}
		while (1)
		{
			position = find(equation.begin(), equation.end(), "+");
			if (position != equation.end())
			{
				ans = S_cal(*(position - 1), *(position), *(position + 1));
				equation.insert(position - 1, to_string(ans));
				position = find(equation.begin(), equation.end(), "+");
				equation.erase(position - 1, position + 2);
			}
			else
			{
				break;
			}
		}
		Ans.push_back(ans);
	}
}
string choose_symbol()
{
	while (1)
	{
		int cho = (rand() % 4) + 1;
		if (cho == 1 && sym[0] == 1)
		{
			return "+";
		}
		else if (cho == 2 && sym[1] == 1)
		{
			return "-";
		}
		else if (cho == 3 && sym[2] == 1)
		{
			return "*";
		}
		else if (cho == 4 && sym[3] == 1)
		{
			return "/";
		}
	}
}
void output()
{
	if (way == 0)
	{
		fstream file1;
		vector<string>::iterator iter;
		file1.open("Question.txt", ios::out);
		for (int i = 0; i < save.size(); i++)
		{
			for (iter = save[i].begin(); iter != save[i].end(); ++iter)
			{
				file1 << *iter;
			}
			file1 << "=" << endl;
		}
		file1.close();
		cout << "��ʽ�Ѳ������ѱ��浽�ļ�,��Ŀ�ͽ���ֿ����棬�����ڶ�ӦĿ¼�²鿴" << endl;
	}
	else if (way == 1)
	{
		cout << "��ʽ�Ѳ�����������ʾ,������ļ���ʽ���棬�����ڶ�ӦĿ¼�²鿴" << endl;
		for (int i = 0; i < save.size(); i++)
		{
			for (int k = 0; k < save[i].size(); k++)
			{
				cout << save[i][k];
			}
			cout << "=" << endl;
		}
	}
	calculate();
	int i = 0;
	fstream file2;
	vector<float>::iterator iter1;
	file2.open("Answer.txt", ios::out);
	for (iter1 = Ans.begin(); iter1 != Ans.end(); ++iter1)
	{
		if (Error.size() != 0 && Error[0] == i)
		{
			file2 << i + 1 << ". " << "������Ŀ�����޴�" << endl;
		}
		else
		{
			file2 << i + 1 << ". " << *iter1 << endl;
		}
		i++;
	}
	file2.close();
}
void produce(int num)
{
	int ins;//�������ŵ�λ��
	int act;//ʵ�����Ӹ���
	int start = 3;//ʽ�����Ӹ�����Сֵ�������ڴ˸���
	int end = 8;//ʽ�����Ӹ������ֵ�������ٴθ���
	if (judge[1] == 0)
	{
		default_random_engine e1(time(0));
		uniform_int_distribution<int> Random1(1, Max);
		for (int i = 0; i < num; i++)
		{
			vector <string> test;
			act = (rand() % (end - start + 1)) + start;
			for (int k = 0; k < act; k++)
			{
				test.push_back(to_string(Random1(e1)));
				test.push_back(choose_symbol());
			}
			test.pop_back();
			if (judge[0] == 1)
			{
				ins = rand() % (act - 1);
				test.insert(test.begin() + ins * 2, "(");
				test.insert(test.begin() + ins * 2 + 4, ")");
			}
			save.push_back(test);
		}
	}
	else
	{
		default_random_engine e2(time(0));
		uniform_real_distribution<double> Random2(-Max, Max);
		for (int i = 0; i < num; i++)
		{
			vector <string> test;
			act = (rand() % (end - start + 1)) + start;
			for (int k = 0; k < act; k++)
			{
				test.push_back(to_string(Random2(e2)));
				test.push_back(choose_symbol());
			}
			test.pop_back();
			if (judge[0] == 1)
			{
				ins = rand() % (act - 1);
				test.insert(test.begin() + ins * 2, "(");
				test.insert(test.begin() + ins * 2 + 4, ")");
			}
			save.push_back(test);
		}
	}
	output();
}
void set_mount()
{
	int i;
	string content;
	cout << "����������Ҫ���ɵ���Ŀ����" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		for (i = 0; i < content.size();)
		{
			if (content[i] >= '0'&&content[i] <= '9')
			{
				Tolsize = Tolsize * 10 + content[i] - 48;
			}
			else
			{
				cout << "����������ݲ����ϱ�׼������������" << endl;
				break;
			}
			i++;
		}
		if (i == content.size())
		{
			break;
		}
		Tolsize = 0;
	}
}
void set_symbol()
{
	string content;
	int i;
	cout << "����ѡ����ʽ����Ҫ�����������,����+��-��*��/���ɣ������������Żᱨ��,�ո�֮������" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		for (i = 0; i < content.size();)
		{
			if (content[i] == '+')
			{
				sym[0] = 1;
			}
			else if (content[i] == '-')
			{
				sym[1] = 1;
			}
			else if (content[i] == '*')
			{
				sym[2] = 1;
			}
			else if (content[i] == '/')
			{
				sym[3] = 1;
			}
			else
			{
				sym[0] = 0;
				sym[1] = 0;
				sym[2] = 0;
				sym[3] = 0;
				cout << "�����������������" << endl;
				break;
			}
			i++;
		}
		if (i == content.size())
		{
			break;
		}
	}
}
void set_max()
{
	string content;
	int i = -1;
	cout << "��������ʽ�г��ֵ����ֵ����ֵ" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		for (i = 0; i < content.size();)
		{
			if (content[i] >= '0'&&content[i] <= '9')
			{
				Max = Max * 10 + content[i] - 48;
			}
			else
			{
				cout << "�����������������" << endl;
				Max = 0;
				break;
			}
			i++;
		}
		if (i == content.size())
		{
			break;
		}
	}
}
void other_set()
{
	string content;
	int i = -1;
	cout << "����ѡ����ʽ���Ƿ�������ţ��Ƿ���С����������λ���ּ��ɣ��������ζ�Ӧ��һ��͵ڶ���,0Ϊ������1Ϊ�����������������ᱨ���ո������" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		if (content.size() != 2)
		{
			cout << "�����������������" << endl;
		}
		else
		{
			for (i = 0; i < content.size();)
			{
				if (content[i] >= '0' || content[i] == '1')
				{
					judge[i] = content[i] - 48;
					i++;
				}
				else
				{
					judge[0] = 0;
					judge[1] = 0;
				}
			}
			if (i == content.size())
			{
				break;
			}
		}
	}
}
void set_way()
{
	string content;
	cout << "��ѡ�������ʽ��0����������ļ��У�1�����������Ļ��" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		if (content.size() != 1)
		{
			cout << "�����������������" << endl;
		}
		else if (content[0] == '0' || content[0] == '1')
		{
			way = content[0] - 48;
			break;
		}
		else
		{
			cout << "�����������������" << endl;
		}
	}
}
int main()
{
	cout << "----------------------------------------" << endl;
	cout << "----��ӭʹ������������Ŀ������С����----" << endl;
	cout << "----------------------------------------" << endl;
	cout << "����֮ǰ��Ҫ�����뼸���������Ա�֤���ɵ���Ŀ��������Ҫ��" << endl;
	while (1)
	{
		set_mount();//������Ŀ����
		set_symbol();//ѡ�������
		set_max();//���������
		other_set();//�����Ƿ�����ţ��Ƿ���С��
		set_way();//ѡ�������ʽ
		produce(Tolsize);
		while (1)
		{
			cout << "�Ƿ����ʹ��,��������1����������0" << endl;
			string j;
			cin >> j;
			if (j == "0")
			{
				cout << "��л��ѡ��ʹ�����ǵ�С�����ټ�" << endl;
				system("pause");
				return 0;
			}
			else if (j == "1")
			{
				cout << "�����ʹ��" << endl;
				break;
			}
			else
			{
				cout << "�����������������" << endl;
			}
		}
	}
}
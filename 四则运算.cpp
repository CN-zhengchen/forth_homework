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
int Tolsize = 0;//题目数量
int sym[4] = { 0 };//存储符号选择
int Max = 0;//存储最大值
int judge[2] = { 0 };//存储括号和小数判断
int way;//输出方式
int flag = 0;//判断题目合理性
vector < vector<string> > save;
vector <float> Ans;//存储答案
vector <int> Error;//存储错误题号
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
	vector<string>::iterator  position;//查找符号的位置
	vector<string>::iterator  position1;//查找另一符号的位置
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
		cout << "算式已产生，已保存到文件,题目和结果分开保存，可以在对应目录下查看" << endl;
	}
	else if (way == 1)
	{
		cout << "算式已产生，如下所示,结果以文件形式保存，可以在对应目录下查看" << endl;
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
			file2 << i + 1 << ". " << "此题题目有误，无答案" << endl;
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
	int ins;//插入括号的位置
	int act;//实际因子个数
	int start = 3;//式中因子个数最小值，可以在此更改
	int end = 8;//式中因子个数最大值，可以再次更改
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
	cout << "请您输入需要生成的题目数量" << endl;
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
				cout << "您输入的内容不符合标准，请重新输入" << endl;
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
	cout << "请您选择算式中想要包含的运算符,输入+，-，*，/即可，输入其他符号会报错,空格之后会忽略" << endl;
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
				cout << "输入错误，请重新输入" << endl;
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
	cout << "请输入算式中出现的数字的最大值" << endl;
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
				cout << "输入错误，请重新输入" << endl;
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
	cout << "请您选择算式中是否包含括号，是否有小数，输入两位数字即可，数字依次对应第一项和第二项,0为包含，1为不包含，输入其他会报错，空格后会忽略" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		if (content.size() != 2)
		{
			cout << "输入错误，请重新输入" << endl;
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
	cout << "请选择输出方式，0代表输出到文件中，1代表输出到屏幕上" << endl;
	while (1)
	{
		content = { "\0" };
		cin >> content;
		if (content.size() != 1)
		{
			cout << "输入错误，请重新输入" << endl;
		}
		else if (content[0] == '0' || content[0] == '1')
		{
			way = content[0] - 48;
			break;
		}
		else
		{
			cout << "输入错误，请重新输入" << endl;
		}
	}
}
int main()
{
	cout << "----------------------------------------" << endl;
	cout << "----欢迎使用四则运算题目生成器小程序----" << endl;
	cout << "----------------------------------------" << endl;
	cout << "生成之前需要您输入几个参数，以保证生成的题目符合您的要求" << endl;
	while (1)
	{
		set_mount();//输入题目数量
		set_symbol();//选择运算符
		set_max();//设置最大数
		other_set();//设置是否带括号，是否有小数
		set_way();//选择输出方式
		produce(Tolsize);
		while (1)
		{
			cout << "是否继续使用,是请输入1，否请输入0" << endl;
			string j;
			cin >> j;
			if (j == "0")
			{
				cout << "感谢您选择并使用我们的小程序，再见" << endl;
				system("pause");
				return 0;
			}
			else if (j == "1")
			{
				cout << "请继续使用" << endl;
				break;
			}
			else
			{
				cout << "输入错误，请重新输入" << endl;
			}
		}
	}
}
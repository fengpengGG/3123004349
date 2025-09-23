#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include <stdio.h>
#include <iostream>
#include "FileHandling.h"
#include"TextPreprocess.h"
#include"Check.h"
using namespace std;

int main(int argc, char* argv[])
{
{
if (argc != 4) //一共有三个输入，所以argc = 4 
	cout << "输入错误，请重新输入" << endl;
	return 1;
}

	string original_path = argv[1];
	string copied_path = argv[2];
	string result_path = argv[3];

	try
	{
		//读取文件
		string original_text = FileHandling::read_file(original_path);//源文件
		string copied_text = FileHandling::read_file(copied_path);	 //抄袭文件

		//文本预处理和分词
		vector<string> original_words = TextPreprocess::process_text(original_text);
		vector<string> copied_words = TextPreprocess::process_text(copied_text);

		//计算相似度
		double similarity = Check::calculate_similarity(original_words, copied_words);

		FileHandling::write_result(result_path, similarity);

		cout << "成功生成结果" << endl;
		system("pause");
		return 0;
	}
	catch (const exception& e) {
		cout << "错误: " << e.what() << endl;
		return 1;
	}
}

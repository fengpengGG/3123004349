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
	if (argc != 4) //һ�����������룬����argc = 4 
	{
		cout << "�����������������" << endl;
		return 1;
	}

	string original_path = argv[1]; //Դ�ļ�·��
	string copied_path = argv[2];   //��Ϯ�ļ�·��
	string result_path = argv[3];   //����ļ�·��

	try
	{
		//��ȡ�ļ�
		string original_text = FileHandling::read_file(original_path);//Դ�ļ�
		string copied_text = FileHandling::read_file(copied_path);	 //��Ϯ�ļ�

		//�ı�Ԥ����ͷִ�
		vector<string> original_words = TextPreprocess::process_text(original_text);
		vector<string> copied_words = TextPreprocess::process_text(copied_text);

		//�������ƶ�
		double similarity = check::calculate_similar(original_words, copied_words);

		FileHandling::write_result(result_path,similarity);
		
		return 0;
	}
	catch (const exception& e) {
		cout << "����: " << e.what() << endl;
		return 1;
	}
}


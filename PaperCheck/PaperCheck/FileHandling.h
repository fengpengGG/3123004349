#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include<iostream>
#include <string>
#include <fstream>
#include <stdexcept> 
#include <locale> //���������Ӧ��ͬ�������������
#include <codecvt> //C++ ��׼���������ַ�����ת����ͷ�ļ�����Ҫ�ṩ��һ��ģ���࣬���ڱ��빦�ܵ�ת��


using namespace std;

class FileHandling
{
public:
	//��ȡ�ļ����ݡ�
	static string read_file(const string& path) {  //·��
		ifstream file(path, ios::in | ios::binary);
		if (!file.is_open())
			throw runtime_error("�޷����ļ�" + path); //�׳�����

		//��ȡ�ļ����ݵ��ַ���
		string content((istreambuf_iterator<char>(file)),  //��ȡ�������ַ�
			istreambuf_iterator<char>());

		file.close();
		return content;
	}

	// д�������ļ���������λС��
	static void write_result(const string& path, double similarity) {
		ofstream file(path);
		if (!file.is_open()) {
			throw runtime_error("�޷���������ļ�: " + path);
		}

		// �����������Ϊ��λС��
		file.precision(2);
		file << fixed << similarity;

		file.close();
	}


};
#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include<iostream>
#include <string>
#include <fstream>
#include <stdexcept> 
#include <locale> //解决程序适应不同地区规则的问题
#include <codecvt> //C++ 标准库中用于字符编码转换的头文件，主要提供了一组模板类，用于编码功能的转换


using namespace std;

class FileHandling
{
public:
	//读取文件内容。
	static string read_file(const string& path) {  //路径
		ifstream file(path, ios::in | ios::binary);
		if (!file.is_open())
			throw runtime_error("无法打开文件" + path); //抛出错误

		//读取文件内容到字符串
		string content((istreambuf_iterator<char>(file)),  //读取连续的字符
			istreambuf_iterator<char>());

		file.close();
		return content;
	}

	// 写入结果到文件，保留两位小数
	static void write_result(const string& path, double similarity) {
		ofstream file(path);
		if (!file.is_open()) {
			throw runtime_error("无法创建输出文件: " + path);
		}

		// 设置输出精度为两位小数
		file.precision(2);
		file << fixed << similarity;

		file.close();
	}


};
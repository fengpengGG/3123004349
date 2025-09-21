#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)

#include <fstream>
#include <string>
#include <stdexcept>
#include <codecvt>
#include <locale>

using namespace std;

class FileHandling {
public:
	// ��ȡ�ļ����ݣ�֧��UTF-8���룩
	static string read_file(const string& file_path) {
		// ���Դ��ļ�
		ifstream file(file_path, ios::binary);
		if (!file.is_open()) {
			throw runtime_error("�޷����ļ�: " + file_path);
		}

		// ��ȡ�ļ�����
		string content((istreambuf_iterator<char>(file)),
			istreambuf_iterator<char>());

		if (content.empty() && !file.eof()) {
			throw runtime_error("��ȡ�ļ�ʧ��: " + file_path);
		}

		return content;
	}

	// д������������λС����
	static void write_result(const string& file_path, double similarity) {
		ofstream file(file_path);
		if (!file.is_open()) {
			throw runtime_error("�޷���������ļ�: " + file_path);
		}

		// ȷ�������0-1֮�䣬��������λС��
		similarity = max(0.0, min(1.0, similarity));
		file.precision(2);
		file << fixed << similarity;

		if (!file.good()) {
			throw runtime_error("д�����ļ�ʧ��: " + file_path);
		}
	}
};

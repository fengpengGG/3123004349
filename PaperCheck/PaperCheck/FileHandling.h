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
	// 读取文件内容（支持UTF-8编码）
	static string read_file(const string& file_path) {
		// 尝试打开文件
		ifstream file(file_path, ios::binary);
		if (!file.is_open()) {
			throw runtime_error("无法打开文件: " + file_path);
		}

		// 读取文件内容
		string content((istreambuf_iterator<char>(file)),
			istreambuf_iterator<char>());

		if (content.empty() && !file.eof()) {
			throw runtime_error("读取文件失败: " + file_path);
		}

		return content;
	}

	// 写入结果（保留两位小数）
	static void write_result(const string& file_path, double similarity) {
		ofstream file(file_path);
		if (!file.is_open()) {
			throw runtime_error("无法创建结果文件: " + file_path);
		}

		// 确保结果在0-1之间，并保留两位小数
		similarity = max(0.0, min(1.0, similarity));
		file.precision(2);
		file << fixed << similarity;

		if (!file.good()) {
			throw runtime_error("写入结果文件失败: " + file_path);
		}
	}
};

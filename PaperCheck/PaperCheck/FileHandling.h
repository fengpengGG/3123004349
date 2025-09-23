#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)

#include <fstream>
#include <string>
#include <stdexcept>
#include <codecvt>
#include <locale>
#include <cstddef>
using namespace std;

class FileHandling {
public:
    // 读取文件内容
    static string read_file(const string& file_path) {
        ifstream file(file_path, ios::binary);
        if (!file.is_open()) {
            throw runtime_error("无法打开文件: " + file_path);
        }

        // 自定义8KB缓冲区
        const size_t BUF_SIZE = 8 * 1024;
        char* io_buf = new char[BUF_SIZE];
        file.rdbuf()->pubsetbuf(io_buf, BUF_SIZE); // 绑定缓冲区到文件流

        // 读取内容（避免临时对象冗余）
        string content;
        content.reserve(BUF_SIZE * 16); // 预分配内存
        char buf[BUF_SIZE];
        while (file.read(buf, BUF_SIZE)) {
            content.append(buf, BUF_SIZE);
        }
        // 读取剩余字节
        content.append(buf, file.gcount());

        // 释放资源+检查错误
        delete[] io_buf;
        if (content.empty() && !file.eof()) {
            throw runtime_error("读取文件失败: " + file_path);
        }
        return content;
    }

    // 写入结果
    static void write_result(const string& file_path, double similarity) {
        ofstream file(file_path);
        if (!file.is_open()) {
            throw runtime_error("无法创建结果文件: " + file_path);
        }

        // 确保相似度在0-1区间，保留两位小数
        similarity = max(0.0, min(1.0, similarity));
        file.precision(2);
        file << fixed << similarity;

        
        file.flush();
        if (!file.good()) {
            throw runtime_error("写入结果文件失败: " + file_path);
        }
    }



};
	

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
    // ��ȡ�ļ�����
    static string read_file(const string& file_path) {
        ifstream file(file_path, ios::binary);
        if (!file.is_open()) {
            throw runtime_error("�޷����ļ�: " + file_path);
        }

        // �Զ���8KB������
        const size_t BUF_SIZE = 8 * 1024;
        char* io_buf = new char[BUF_SIZE];
        file.rdbuf()->pubsetbuf(io_buf, BUF_SIZE); // �󶨻��������ļ���

        // ��ȡ���ݣ�������ʱ�������ࣩ
        string content;
        content.reserve(BUF_SIZE * 16); // Ԥ�����ڴ�
        char buf[BUF_SIZE];
        while (file.read(buf, BUF_SIZE)) {
            content.append(buf, BUF_SIZE);
        }
        // ��ȡʣ���ֽ�
        content.append(buf, file.gcount());

        // �ͷ���Դ+������
        delete[] io_buf;
        if (content.empty() && !file.eof()) {
            throw runtime_error("��ȡ�ļ�ʧ��: " + file_path);
        }
        return content;
    }

    // д����
    static void write_result(const string& file_path, double similarity) {
        ofstream file(file_path);
        if (!file.is_open()) {
            throw runtime_error("�޷���������ļ�: " + file_path);
        }

        // ȷ�����ƶ���0-1���䣬������λС��
        similarity = max(0.0, min(1.0, similarity));
        file.precision(2);
        file << fixed << similarity;

        
        file.flush();
        if (!file.good()) {
            throw runtime_error("д�����ļ�ʧ��: " + file_path);
        }
    }



};
	

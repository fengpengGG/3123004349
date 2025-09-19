#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include<iostream>
#include<string>
#include<unordered_set> //�����˹��ı���������Ĵ�
#include<cctype> //�ַ�������ת���ĺ��������ڼ���Ƿ�Ϊ��ĸ�����֣�ת����Сд
#include<algorithm>
#include<vector>

using namespace std;

//�ı�Ԥ�����ѱ���������Ĵ�ȥ����ת����Сд��
class  TextPreprocess
{
public:
	static vector<string> process_text(const string& text) {
		vector<string> words;
		string current_word;


		for (long long i = 0; i < text.size();) {

			//������Ӣ����ı�
			if ((static_cast<unsigned char>(text[i]) & 0xE0) == 0xc0) {
				//�жϵ�ǰ�ַ��Ƿ�Ϊһ�� 2 �ֽ� UTF-8 �������ʼ�ֽ�

				if (i + 1 < text.size()) {
					current_word += text.substr(i, 2); //��ȡ�ַ���
					i += 2;
				}
				else {
					i++;
				}
			}
			else if ((static_cast<unsigned char>(text[i]) & 0xF0) == 0xE0) {
				//�жϵ�ǰ�ַ��Ƿ�Ϊһ�� 3�ֽ� UTF-8 �������ʼ�ֽ� �󲿷�Ϊ�����ַ�

				if (i + 2 < text.size()) {
					current_word += text.substr(i, 3); //��ȡ�ַ���
					i += 3;
				}
				else {
					i++;
				}
			}
			else if ((static_cast<unsigned char>(text[i]) & 0xF8) == 0xF0) {
				//�жϵ�ǰ�ַ��Ƿ�Ϊһ�� 4�ֽ� UTF-8 �������ʼ�ֽ� 

				if (i + 3 < text.size()) {
					current_word += text.substr(i, 4); //��ȡ�ַ���
					i += 4;
				}
				else {
					i++;
				}

			}
			else if (isalnum(static_cast<unsigned char>(text[i]))) { //isalnum �Ƿ�Ϊ��ĸ
				//������ĸ������
				current_word += text[i];
				i++;
			}
			else
			{
				//�����ָ�����������ǰ��
				if (!current_word.empty()) {
					//����ͣ�ô�
					if (!is_stop_word(current_word))
					{
						transform(current_word.begin(), current_word.end(), current_word.begin(), ::tolower);
						//Ӣ��תСд
						words.push_back(current_word);
					}
					current_word.clear();
				}
				i++;
			}

		}
		//������һ����
		if (!current_word.empty() && !is_stop_word(current_word)) {
			transform(current_word.begin(), current_word.end(), current_word.begin(), ::tolower);
			words.push_back(current_word);
		}
		return words;
	}
private:

	//����Ƿ�Ϊͣ�ô�
	static bool is_stop_word(const string& word) {
		static const unordered_set<string> stop_words = { //�����ϣ��������
			 "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "һ",
			"һ��", "��", "Ҳ", "��", "��", "˵", "Ҫ", "ȥ", "��", "��", "��", "û��",
			"��", "��", "�Լ�", "��", "����", "����", "����", "ʱ��", "��", "��", "��"
		};
		return stop_words.find(word) != stop_words.end();
	}

};
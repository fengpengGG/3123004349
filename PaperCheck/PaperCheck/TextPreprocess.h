#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include<iostream>
#include<string>
#include<unordered_set> //快速滤过文本中无意义的词
#include<cctype> //字符分类与转换的函数，用于检查是否为字母、数字，转换大小写
#include<algorithm>
#include<vector>

using namespace std;

//文本预处理，把标点和无意义的词去除，转化大小写。
class  TextPreprocess
{
public:
	static vector<string> process_text(const string& text) {
		vector<string> words;
		string current_word;


		for (long long i = 0; i < text.size();) {

			//处理中英混合文本
			if ((static_cast<unsigned char>(text[i]) & 0xE0) == 0xc0) {
				//判断当前字符是否为一个 2 字节 UTF-8 编码的起始字节

				if (i + 1 < text.size()) {
					current_word += text.substr(i, 2); //截取字符串
					i += 2;
				}
				else {
					i++;
				}
			}
			else if ((static_cast<unsigned char>(text[i]) & 0xF0) == 0xE0) {
				//判断当前字符是否为一个 3字节 UTF-8 编码的起始字节 大部分为中文字符

				if (i + 2 < text.size()) {
					current_word += text.substr(i, 3); //截取字符串
					i += 3;
				}
				else {
					i++;
				}
			}
			else if ((static_cast<unsigned char>(text[i]) & 0xF8) == 0xF0) {
				//判断当前字符是否为一个 4字节 UTF-8 编码的起始字节 

				if (i + 3 < text.size()) {
					current_word += text.substr(i, 4); //截取字符串
					i += 4;
				}
				else {
					i++;
				}

			}
			else if (isalnum(static_cast<unsigned char>(text[i]))) { //isalnum 是否为字母
				//处理字母和数字
				current_word += text[i];
				i++;
			}
			else
			{
				//遇到分隔符，结束当前词
				if (!current_word.empty()) {
					//过滤停用词
					if (!is_stop_word(current_word))
					{
						transform(current_word.begin(), current_word.end(), current_word.begin(), ::tolower);
						//英文转小写
						words.push_back(current_word);
					}
					current_word.clear();
				}
				i++;
			}

		}
		//添加最后一个词
		if (!current_word.empty() && !is_stop_word(current_word)) {
			transform(current_word.begin(), current_word.end(), current_word.begin(), ::tolower);
			words.push_back(current_word);
		}
		return words;
	}
private:

	//检查是否为停用词
	static bool is_stop_word(const string& word) {
		static const unordered_set<string> stop_words = { //无序哈希集合容器
			 "的", "了", "是", "在", "我", "有", "和", "就", "不", "人", "都", "一",
			"一个", "上", "也", "很", "到", "说", "要", "去", "你", "会", "着", "没有",
			"看", "好", "自己", "这", "今天", "天气", "晚上", "时间", "月", "日", "年"
		};
		return stop_words.find(word) != stop_words.end();
	}

};
#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include <unordered_map>//词频统计
#include <unordered_set>//储存唯一元素

using namespace std;

/*

将 “文本” 转换为 “数学向量”，再通过向量夹角衡量相似度：
两个向量的夹角越小（余弦值越接近 1），说明文本用词越相似，内容相关性越高。

*/

class check{
public:
	static double calculate_similar(const vector<string> word1, const vector<string> word2)
	{

		//建立词频映射
		unordered_map<string, int> freq1 = get_word_f(word1);	
		unordered_map<string, int> freq2 = get_word_f(word2);

		//获取所有不重复的词
		unordered_set<string> all_words;
		for (const auto& pair : freq1) all_words.insert(pair.first);
		for (const auto& pair : freq2) all_words.insert(pair.first);

		//计算点积
		double dot_product = 0.0;
		//计算向量的平方
		double norm1_sq = 0.0, norm2_sq = 0.0;

		for (const string& word : all_words) {
			int count1 = freq1.count(word) ? freq1[word] : 0;
			int count2 = freq2.count(word) ? freq2[word] : 0;
		

			dot_product += count1 * count2;
			norm1_sq += count1 * count1;
			norm2_sq += count2 * count2;
		}
		//计算模长
		double norm1 = sqrt(norm1_sq);
		double norm2 = sqrt(norm2_sq);
		
		if (norm1 == 0 || norm2 == 0)
			return 0.0;
		//返回余弦相似度
		return dot_product / (norm1 * norm2);
	}
	
	

private:

	//计算词频
	static unordered_map<string, int>get_word_f(const vector<string>& words) {
		unordered_map<std::string, int> f;
		for (const string& word : words) {
			f[word]++;
		}
		return f;

	}

};
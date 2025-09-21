#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*
采用了加权 Jaccard 系数算法
 Jaccard 系数用于计算两个集合的相似度，公式为：J(A,B) = |A∩B| / |A∪B|

*/

class Check {
public:
	// 计算文本相似度
	static double calculate_similarity(const vector<string>& words1, const vector<string>& words2) {
		// 处理空文本情况
		if (words1.empty() && words2.empty()) return 1.0;
		if (words1.empty() || words2.empty()) return 0.0;

		// 统计词频
		unordered_map<string, int> freq1, freq2;
		for (const string& word : words1) freq1[word]++;
		for (const string& word : words2) freq2[word]++;

		// 计算交集和并集
		double intersection = 0.0;
		double union_set = 0.0;

		// 处理第一个文本中的词
		for (const auto& pair : freq1) {
			const string& word = pair.first;
			int count1 = pair.second;

			if (freq2.find(word) != freq2.end()) {
				// 交集取最小频次
				intersection += min(count1, freq2[word]);
				// 并集取最大频次
				union_set += max(count1, freq2[word]);
				// 从第二个文本中移除已处理的词
				freq2.erase(word);
			}
			else {
				// 只在第一个文本中出现的词
				union_set += count1;
			}
		}

		// 处理只在第二个文本中出现的词
		for (const auto& pair : freq2) {
			union_set += pair.second;
		}

		// 计算相似度
		if (union_set == 0) return 0.0;
		return intersection / union_set;
	}
};

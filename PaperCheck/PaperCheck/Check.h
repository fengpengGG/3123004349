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
优化：将双哈希表改为单哈希表，减少1次遍历+erase操作，预分配桶大小避免rehash
 Jaccard 系数用于计算两个集合的相似度，公式为：J(A,B) = |A∩B| / |A∪B|
*/



class Check {
public:
	// 计算文本相似度
	static double calculate_similarity(const vector<string>& words1, const vector<string>& words2) {
		// 处理空文本情况
		if (words1.empty() && words2.empty()) return 1.0;
		if (words1.empty() || words2.empty()) return 0.0;

		// 优化：单哈希表存储{词: {text1词频, text2词频}}，预分配桶大小避免rehash
		unordered_map<string, pair<int, int>> freq_map;
		freq_map.reserve(max(words1.size(), words2.size()) * 1.1); // 1.1倍预留空间

		// 统计第一个文本的词频
		for (const string& word : words1) {
			freq_map[word].first++;
		}
		// 统计第二个文本的词频
		for (const string& word : words2) {
			freq_map[word].second++;
		}

		// 优化：一次遍历计算交集和并集（原代码需2次遍历+erase）
		double intersection = 0.0;
		double union_set = 0.0;
		for (const auto& [word, cnt_pair] : freq_map) { // C++17结构化绑定
			intersection += min(cnt_pair.first, cnt_pair.second);
			union_set += max(cnt_pair.first, cnt_pair.second);
		}

		return (union_set == 0) ? 0.0 : intersection / union_set;

		
	}
};

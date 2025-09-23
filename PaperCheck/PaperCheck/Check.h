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
�����˼�Ȩ Jaccard ϵ���㷨
�Ż�����˫��ϣ���Ϊ����ϣ������1�α���+erase������Ԥ����Ͱ��С����rehash
 Jaccard ϵ�����ڼ����������ϵ����ƶȣ���ʽΪ��J(A,B) = |A��B| / |A��B|
*/



class Check {
public:
	// �����ı����ƶ�
	static double calculate_similarity(const vector<string>& words1, const vector<string>& words2) {
		// ������ı����
		if (words1.empty() && words2.empty()) return 1.0;
		if (words1.empty() || words2.empty()) return 0.0;

		// �Ż�������ϣ��洢{��: {text1��Ƶ, text2��Ƶ}}��Ԥ����Ͱ��С����rehash
		unordered_map<string, pair<int, int>> freq_map;
		freq_map.reserve(max(words1.size(), words2.size()) * 1.1); // 1.1��Ԥ���ռ�

		// ͳ�Ƶ�һ���ı��Ĵ�Ƶ
		for (const string& word : words1) {
			freq_map[word].first++;
		}
		// ͳ�Ƶڶ����ı��Ĵ�Ƶ
		for (const string& word : words2) {
			freq_map[word].second++;
		}

		// �Ż���һ�α������㽻���Ͳ�����ԭ������2�α���+erase��
		double intersection = 0.0;
		double union_set = 0.0;
		for (const auto& [word, cnt_pair] : freq_map) { // C++17�ṹ����
			intersection += min(cnt_pair.first, cnt_pair.second);
			union_set += max(cnt_pair.first, cnt_pair.second);
		}

		return (union_set == 0) ? 0.0 : intersection / union_set;

		
	}
};

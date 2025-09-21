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
 Jaccard ϵ�����ڼ����������ϵ����ƶȣ���ʽΪ��J(A,B) = |A��B| / |A��B|

*/

class Check {
public:
	// �����ı����ƶ�
	static double calculate_similarity(const vector<string>& words1, const vector<string>& words2) {
		// ������ı����
		if (words1.empty() && words2.empty()) return 1.0;
		if (words1.empty() || words2.empty()) return 0.0;

		// ͳ�ƴ�Ƶ
		unordered_map<string, int> freq1, freq2;
		for (const string& word : words1) freq1[word]++;
		for (const string& word : words2) freq2[word]++;

		// ���㽻���Ͳ���
		double intersection = 0.0;
		double union_set = 0.0;

		// �����һ���ı��еĴ�
		for (const auto& pair : freq1) {
			const string& word = pair.first;
			int count1 = pair.second;

			if (freq2.find(word) != freq2.end()) {
				// ����ȡ��СƵ��
				intersection += min(count1, freq2[word]);
				// ����ȡ���Ƶ��
				union_set += max(count1, freq2[word]);
				// �ӵڶ����ı����Ƴ��Ѵ���Ĵ�
				freq2.erase(word);
			}
			else {
				// ֻ�ڵ�һ���ı��г��ֵĴ�
				union_set += count1;
			}
		}

		// ����ֻ�ڵڶ����ı��г��ֵĴ�
		for (const auto& pair : freq2) {
			union_set += pair.second;
		}

		// �������ƶ�
		if (union_set == 0) return 0.0;
		return intersection / union_set;
	}
};

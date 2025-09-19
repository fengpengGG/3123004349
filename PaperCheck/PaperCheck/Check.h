#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)

#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include <unordered_map>//��Ƶͳ��
#include <unordered_set>//����ΨһԪ��

using namespace std;

/*

�� ���ı��� ת��Ϊ ����ѧ����������ͨ�������нǺ������ƶȣ�
���������ļн�ԽС������ֵԽ�ӽ� 1����˵���ı��ô�Խ���ƣ����������Խ�ߡ�

*/

class check{
public:
	static double calculate_similar(const vector<string> word1, const vector<string> word2)
	{

		//������Ƶӳ��
		unordered_map<string, int> freq1 = get_word_f(word1);	
		unordered_map<string, int> freq2 = get_word_f(word2);

		//��ȡ���в��ظ��Ĵ�
		unordered_set<string> all_words;
		for (const auto& pair : freq1) all_words.insert(pair.first);
		for (const auto& pair : freq2) all_words.insert(pair.first);

		//������
		double dot_product = 0.0;
		//����������ƽ��
		double norm1_sq = 0.0, norm2_sq = 0.0;

		for (const string& word : all_words) {
			int count1 = freq1.count(word) ? freq1[word] : 0;
			int count2 = freq2.count(word) ? freq2[word] : 0;
		

			dot_product += count1 * count2;
			norm1_sq += count1 * count1;
			norm2_sq += count2 * count2;
		}
		//����ģ��
		double norm1 = sqrt(norm1_sq);
		double norm2 = sqrt(norm2_sq);
		
		if (norm1 == 0 || norm2 == 0)
			return 0.0;
		//�����������ƶ�
		return dot_product / (norm1 * norm2);
	}
	
	

private:

	//�����Ƶ
	static unordered_map<string, int>get_word_f(const vector<string>& words) {
		unordered_map<std::string, int> f;
		for (const string& word : words) {
			f[word]++;
		}
		return f;

	}

};
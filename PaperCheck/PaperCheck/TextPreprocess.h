

#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <algorithm>
using namespace std;

class TextPreprocess {
public:
    // �ı�Ԥ�������������Ż���Ԥ��������vector�ռ䣬�������ݿ�����
    static vector<string> process_text(const string& text) {
        if (text.empty()) return {};
        // Ԥ������������3�ֽ�/�ʣ�Ӣ��5�ַ�/�ʣ�Ԥ����1.2���ռ�������ݣ�
        const size_t est_word_cnt = (text.size() + 2) / 3 * 1.2;

        // 1. ȫ���ת��
        string converted = full_width_to_half_width(text);
        // 2. ȥ�������ź������ַ�
        string cleaned = remove_punctuation(converted);
        // 3. �ִʴ��������Ż���string_view+Ԥ���䣩
        vector<string> words;
        words.reserve(est_word_cnt);
        words = segment_words(cleaned);
        // 4. ͬ��ʹ�һ��
        normalize_synonyms(words);
        // 5. ����ͣ�ôʣ��Ż���Ԥ�������ռ䣩
        vector<string> filtered;
        filtered.reserve(words.size() * 0.8); // Ԥ��80%�ʱ���
        filtered = filter_stop_words(words);

        return filtered;
    }

private:
    // ȫ���ת�����߼����䣬��ҪԤ�����裩
    static string full_width_to_half_width(const string& text) {
        string result;
        result.reserve(text.size()); // Ԥ����ͬ���볤�ȿռ�
        for (char c : text) {
            if (c == 0xA3) continue; // ����ȫ��ǰ����
            if (c >= 0xFF01 && c <= 0xFF5E) {
                result += c - 0xFEE0; // ȫ�ǡ����
            }
            else if (c == 0x3000) {
                result += ' '; // ȫ�ǿո����ǿո�
            }
            else {
                result += c;
            }
        }
        return result;
    }

    // ȥ�������ź������ַ����߼����䣬�Ż�����isalnum��Ӣ���жϣ�
    static string remove_punctuation(const string& text) {
        string result;
        result.reserve(text.size());
        for (char c : text) {
            unsigned char uc = static_cast<unsigned char>(c);
            // ��������ĸ/����/�����ַ�/�ո�
            if (isalnum(uc) || uc == 0x20 || uc >= 0x80) {
                result += c;
            }
        }
        return result;
    }

    // ���ķִ�
    static vector<string> segment_words(const string& text) {
        vector<string> words;
        const size_t text_len = text.size();
        string_view text_view(text); // �޿�������ԭ�ı�
        size_t i = 0;

        // ��չ���ôʵ䣬֧��3�ִ�������ִʿ����ȣ�
        static const unordered_set<string_view> common_words = {
            "����", "������", "����", "����", "����", "��",
            "����", "��", "Ҫȥ", "����Ӱ", "��Ӱ", "��������", "����Ӱȥ"
        };

        while (i < text_len) {
            unsigned char c = static_cast<unsigned char>(text[i]);
            if (c >= 0x80) { // �����ַ���UTF-8ռ3�ֽڣ�
                // �Ż����ƥ����ԣ�����3�֡�2�֡�1�֣�
                bool matched = false;
                // ����3�ִ��9�ֽڣ�
                if (i + 9 <= text_len) {
                    string_view three_chars = text_view.substr(i, 9);
                    if (common_words.count(three_chars)) {
                        words.emplace_back(three_chars); // ֱ�ӹ��죬�޿���
                        i += 9;
                        matched = true;
                    }
                }
                // ����2�ִ��6�ֽڣ�
                if (!matched && i + 6 <= text_len) {
                    string_view two_chars = text_view.substr(i, 6);
                    if (common_words.count(two_chars)) {
                        words.emplace_back(two_chars);
                        i += 6;
                        matched = true;
                    }
                }
                // ����1�ִ��3�ֽڣ�
                if (!matched && i + 3 <= text_len) {
                    string_view one_char = text_view.substr(i, 3);
                    words.emplace_back(one_char);
                    i += 3;
                }
                else if (!matched) {
                    i++; // ��Ч�ַ�����
                }
            }
            else if (isalnum(c)) { // Ӣ��/����
                size_t start = i;
                // ������ȡӢ��/����
                while (i < text_len && isalnum(static_cast<unsigned char>(text[i]))) {
                    i++;
                }
                string word = text_view.substr(start, i - start).data();
                transform(word.begin(), word.end(), word.begin(), ::tolower); // תСд
                words.emplace_back(move(word)); // �ƶ����壬���ٿ���
            }
            else {
                i++; // �ո�/��Ч�ַ�����
            }
        }
        return words;
    }

    // ͬ��ʹ�һ��
    static void normalize_synonyms(vector<string>& words) {
        static const unordered_map<string_view, string> synonyms = {
            {"����", "������"}, {"����", "��"}, {"��Ӱ", "����Ӱ"},
            {"�����", "������"}, {"ӰƬ", "��Ӱ"}
        };
        for (string& word : words) {
            auto it = synonyms.find(word);
            if (it != synonyms.end()) {
                word = it->second;
            }
        }
    }

    // ����ͣ�ô�
    static vector<string> filter_stop_words(const vector<string>& words) {
        vector<string> result;
        static const unordered_set<string_view> stop_words = {
            "��", "��", "��", "��", "��", "��", "��", "��", "��", "һ", "һ��",
            "��", "Ҳ", "��", "��", "˵", "Ҫ", "ȥ", "��", "��", "��", "û��",
            "��", "�Լ�", "��"
        };
        for (const string& word : words) {
            if (stop_words.find(word) == stop_words.end() && !word.empty()) {
                result.emplace_back(word);
            }
        }
        return result;
    }
};
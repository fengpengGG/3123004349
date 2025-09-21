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
    // �ı�Ԥ����������
    static vector<string> process_text(const string& text) {
        // 1. ȫ���ת��
        string converted = full_width_to_half_width(text);

        // 2. ȥ�������ź������ַ�
        string cleaned = remove_punctuation(converted);

        // 3. �ִʴ���
        vector<string> words = segment_words(cleaned);

        // 4. ͬ��ʹ�һ��
        normalize_synonyms(words);

        // 5. ����ͣ�ô�
        vector<string> filtered = filter_stop_words(words);

        return filtered;
    }

private:
    // ȫ���ת��
    static string full_width_to_half_width(const string& text) {
        string result;
        for (char c : text) {
            // ȫ��ת��ǣ�ֻ�������ַ���
            if (c == 0xA3) continue; // ȫ�Ƿ���ǰ���ַ�
            if (c >= 0xFF01 && c <= 0xFF5E) {
                result += c - 0xFEE0;
            }
            else if (c == 0x3000) { // ȫ�ǿո�
                result += ' ';
            }
            else {
                result += c;
            }
        }
        return result;
    }

    // ȥ�������ź������ַ�
    static string remove_punctuation(const string& text) {
        string result;
        for (char c : text) {
            unsigned char uc = static_cast<unsigned char>(c);
            // ������ĸ�����֡������ַ��Ϳո�
            if ((uc >= 0x41 && uc <= 0x5A) || // A-Z
                (uc >= 0x61 && uc <= 0x7A) || // a-z
                (uc >= 0x30 && uc <= 0x39) || // 0-9
                uc == 0x20 || // �ո�
                uc >= 0x80) { // �����ַ���UTF-8��
                result += c;
            }
        }
        return result;
    }

    // ���ķִʣ����ڴʵ�ļ򵥷ִʣ�
    static vector<string> segment_words(const string& text) {
        vector<string> words;
        size_t i = 0;
        const size_t text_len = text.size();

        // �������Ĵ���ʵ䣨����չ��
        static const unordered_set<string> common_words = {
            "����", "������", "����", "����", "����", "��",
            "����", "��", "Ҫȥ", "����Ӱ", "��Ӱ"
        };

        while (i < text_len) {
            unsigned char c = static_cast<unsigned char>(text[i]);
            bool is_chinese = (c >= 0x80);

            if (is_chinese) {
                // ����ƥ��3�ֽ����Ĵ��������
                if (i + 6 <= text_len) { // ����ƥ��2��������ɵĴ�
                    string two_chars = text.substr(i, 6);
                    if (common_words.find(two_chars) != common_words.end()) {
                        words.push_back(two_chars);
                        i += 6;
                        continue;
                    }
                }

                // ����ƥ�䵥�����֣�3�ֽ�UTF-8��
                if (i + 3 <= text_len) {
                    string one_char = text.substr(i, 3);
                    words.push_back(one_char);
                    i += 3;
                    continue;
                }
            }
            // Ӣ�Ļ�����
            else if ((c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9')) {
                size_t start = i;
                while (i < text_len) {
                    unsigned char curr = static_cast<unsigned char>(text[i]);
                    if (!((curr >= 'A' && curr <= 'Z') ||
                        (curr >= 'a' && curr <= 'z') ||
                        (curr >= '0' && curr <= '9'))) {
                        break;
                    }
                    i++;
                }
                string word = text.substr(start, i - start);
                // תΪСд
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                words.push_back(word);
                continue;
            }
            // �ո�������ַ�
            else {
                i++;
            }
        }

        return words;
    }

    // ͬ��ʹ�һ��
    static void normalize_synonyms(vector<string>& words) {
        // ͬ���ӳ�������չ��
        static const unordered_map<string, string> synonyms = {
            {"����", "������"},
            {"����", "��"},
            {"��Ӱ", "����Ӱ"},
            {"�����", "������"}
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
        // ͣ�ôʱ�ֻ����������ʻ㣩
        static const unordered_set<string> stop_words = {
            "��", "��", "��", "��", "��", "��", "��", "��", "��", "һ", "һ��",
            "��", "Ҳ", "��", "��", "˵", "Ҫ", "ȥ", "��", "��", "��", "û��",
            "��", "�Լ�", "��"
        };

        for (const string& word : words) {
            if (stop_words.find(word) == stop_words.end() && !word.empty()) {
                result.push_back(word);
            }
        }

        return result;
    }
};

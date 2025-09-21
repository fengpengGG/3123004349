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
    // 文本预处理主函数
    static vector<string> process_text(const string& text) {
        // 1. 全半角转换
        string converted = full_width_to_half_width(text);

        // 2. 去除标点符号和特殊字符
        string cleaned = remove_punctuation(converted);

        // 3. 分词处理
        vector<string> words = segment_words(cleaned);

        // 4. 同义词归一化
        normalize_synonyms(words);

        // 5. 过滤停用词
        vector<string> filtered = filter_stop_words(words);

        return filtered;
    }

private:
    // 全半角转换
    static string full_width_to_half_width(const string& text) {
        string result;
        for (char c : text) {
            // 全角转半角（只处理常见字符）
            if (c == 0xA3) continue; // 全角符号前导字符
            if (c >= 0xFF01 && c <= 0xFF5E) {
                result += c - 0xFEE0;
            }
            else if (c == 0x3000) { // 全角空格
                result += ' ';
            }
            else {
                result += c;
            }
        }
        return result;
    }

    // 去除标点符号和特殊字符
    static string remove_punctuation(const string& text) {
        string result;
        for (char c : text) {
            unsigned char uc = static_cast<unsigned char>(c);
            // 保留字母、数字、中文字符和空格
            if ((uc >= 0x41 && uc <= 0x5A) || // A-Z
                (uc >= 0x61 && uc <= 0x7A) || // a-z
                (uc >= 0x30 && uc <= 0x39) || // 0-9
                uc == 0x20 || // 空格
                uc >= 0x80) { // 中文字符（UTF-8）
                result += c;
            }
        }
        return result;
    }

    // 中文分词（基于词典的简单分词）
    static vector<string> segment_words(const string& text) {
        vector<string> words;
        size_t i = 0;
        const size_t text_len = text.size();

        // 常用中文词语词典（可扩展）
        static const unordered_set<string> common_words = {
            "今天", "星期天", "周天", "天气", "晴朗", "晴",
            "晚上", "我", "要去", "看电影", "电影"
        };

        while (i < text_len) {
            unsigned char c = static_cast<unsigned char>(text[i]);
            bool is_chinese = (c >= 0x80);

            if (is_chinese) {
                // 尝试匹配3字节中文词语（常见）
                if (i + 6 <= text_len) { // 尝试匹配2个汉字组成的词
                    string two_chars = text.substr(i, 6);
                    if (common_words.find(two_chars) != common_words.end()) {
                        words.push_back(two_chars);
                        i += 6;
                        continue;
                    }
                }

                // 尝试匹配单个汉字（3字节UTF-8）
                if (i + 3 <= text_len) {
                    string one_char = text.substr(i, 3);
                    words.push_back(one_char);
                    i += 3;
                    continue;
                }
            }
            // 英文或数字
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
                // 转为小写
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                words.push_back(word);
                continue;
            }
            // 空格或其他字符
            else {
                i++;
            }
        }

        return words;
    }

    // 同义词归一化
    static void normalize_synonyms(vector<string>& words) {
        // 同义词映射表（可扩展）
        static const unordered_map<string, string> synonyms = {
            {"周天", "星期天"},
            {"晴朗", "晴"},
            {"观影", "看电影"},
            {"礼拜天", "星期天"}
        };

        for (string& word : words) {
            auto it = synonyms.find(word);
            if (it != synonyms.end()) {
                word = it->second;
            }
        }
    }

    // 过滤停用词
    static vector<string> filter_stop_words(const vector<string>& words) {
        vector<string> result;
        // 停用词表（只过滤无意义词汇）
        static const unordered_set<string> stop_words = {
            "的", "了", "在", "是", "有", "和", "就", "不", "都", "一", "一个",
            "上", "也", "很", "到", "说", "要", "去", "你", "会", "着", "没有",
            "好", "自己", "这"
        };

        for (const string& word : words) {
            if (stop_words.find(word) == stop_words.end() && !word.empty()) {
                result.push_back(word);
            }
        }

        return result;
    }
};

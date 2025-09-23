

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
    // 文本预处理主函数（优化：预分配所有vector空间，减少扩容拷贝）
    static vector<string> process_text(const string& text) {
        if (text.empty()) return {};
        // 预估词数（中文3字节/词，英文5字符/词，预分配1.2倍空间避免扩容）
        const size_t est_word_cnt = (text.size() + 2) / 3 * 1.2;

        // 1. 全半角转换
        string converted = full_width_to_half_width(text);
        // 2. 去除标点符号和特殊字符
        string cleaned = remove_punctuation(converted);
        // 3. 分词处理（核心优化：string_view+预分配）
        vector<string> words;
        words.reserve(est_word_cnt);
        words = segment_words(cleaned);
        // 4. 同义词归一化
        normalize_synonyms(words);
        // 5. 过滤停用词（优化：预分配结果空间）
        vector<string> filtered;
        filtered.reserve(words.size() * 0.8); // 预估80%词保留
        filtered = filter_stop_words(words);

        return filtered;
    }

private:
    // 全半角转换（逻辑不变，必要预处理步骤）
    static string full_width_to_half_width(const string& text) {
        string result;
        result.reserve(text.size()); // 预分配同输入长度空间
        for (char c : text) {
            if (c == 0xA3) continue; // 过滤全角前导符
            if (c >= 0xFF01 && c <= 0xFF5E) {
                result += c - 0xFEE0; // 全角→半角
            }
            else if (c == 0x3000) {
                result += ' '; // 全角空格→半角空格
            }
            else {
                result += c;
            }
        }
        return result;
    }

    // 去除标点符号和特殊字符（逻辑不变，优化：用isalnum简化英文判断）
    static string remove_punctuation(const string& text) {
        string result;
        result.reserve(text.size());
        for (char c : text) {
            unsigned char uc = static_cast<unsigned char>(c);
            // 保留：字母/数字/中文字符/空格
            if (isalnum(uc) || uc == 0x20 || uc >= 0x80) {
                result += c;
            }
        }
        return result;
    }

    // 中文分词
    static vector<string> segment_words(const string& text) {
        vector<string> words;
        const size_t text_len = text.size();
        string_view text_view(text); // 无拷贝引用原文本
        size_t i = 0;

        // 扩展常用词典，支持3字词语（提升分词颗粒度）
        static const unordered_set<string_view> common_words = {
            "今天", "星期天", "周天", "天气", "晴朗", "晴",
            "晚上", "我", "要去", "看电影", "电影", "今天天气", "看电影去"
        };

        while (i < text_len) {
            unsigned char c = static_cast<unsigned char>(text[i]);
            if (c >= 0x80) { // 中文字符（UTF-8占3字节）
                // 优化：最长匹配策略（先试3字→2字→1字）
                bool matched = false;
                // 尝试3字词语（9字节）
                if (i + 9 <= text_len) {
                    string_view three_chars = text_view.substr(i, 9);
                    if (common_words.count(three_chars)) {
                        words.emplace_back(three_chars); // 直接构造，无拷贝
                        i += 9;
                        matched = true;
                    }
                }
                // 尝试2字词语（6字节）
                if (!matched && i + 6 <= text_len) {
                    string_view two_chars = text_view.substr(i, 6);
                    if (common_words.count(two_chars)) {
                        words.emplace_back(two_chars);
                        i += 6;
                        matched = true;
                    }
                }
                // 尝试1字词语（3字节）
                if (!matched && i + 3 <= text_len) {
                    string_view one_char = text_view.substr(i, 3);
                    words.emplace_back(one_char);
                    i += 3;
                }
                else if (!matched) {
                    i++; // 无效字符跳过
                }
            }
            else if (isalnum(c)) { // 英文/数字
                size_t start = i;
                // 连续读取英文/数字
                while (i < text_len && isalnum(static_cast<unsigned char>(text[i]))) {
                    i++;
                }
                string word = text_view.substr(start, i - start).data();
                transform(word.begin(), word.end(), word.begin(), ::tolower); // 转小写
                words.emplace_back(move(word)); // 移动语义，减少拷贝
            }
            else {
                i++; // 空格/无效字符跳过
            }
        }
        return words;
    }

    // 同义词归一化
    static void normalize_synonyms(vector<string>& words) {
        static const unordered_map<string_view, string> synonyms = {
            {"周天", "星期天"}, {"晴朗", "晴"}, {"观影", "看电影"},
            {"礼拜天", "星期天"}, {"影片", "电影"}
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
        static const unordered_set<string_view> stop_words = {
            "的", "了", "在", "是", "有", "和", "就", "不", "都", "一", "一个",
            "上", "也", "很", "到", "说", "要", "去", "你", "会", "着", "没有",
            "好", "自己", "这"
        };
        for (const string& word : words) {
            if (stop_words.find(word) == stop_words.end() && !word.empty()) {
                result.emplace_back(word);
            }
        }
        return result;
    }
};
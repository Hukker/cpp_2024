#pragma once
#include <string>

class Subject {
public:
    std::string replace_word(const std::string& text, const std::string& old_word, const std::string& new_word);
    std::string search_phrase(const std::string& text, const std::string& phrase);
    std::string capitalize_words(const std::string& text);
    std::string capitalize_sentences(const std::string& text);
    std::string add_sentence_numbers(const std::string& text);
};

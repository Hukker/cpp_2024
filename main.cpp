#include <iostream>
#include <fstream>
#include <sstream>
#include "subject.h"
#include "engine.h"

std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Failed to open the file: " << file_path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void write_file(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path);
    if (!file) {
        std::cerr << "Failed to open the file for writing: " << file_path << std::endl;
        return;
    }
    file << content;
}

int main() {
    Subject subject;
    Wrapper wrapper1(&subject, &Subject::replace_word, { 
        {"text", ""}, 
        {"old_word", ""}, 
        {"new_word", ""} 
    });
    Wrapper wrapper2(&subject, &Subject::search_phrase, { 
        {"text", ""}, 
        {"phrase", ""} 
    });
    Wrapper wrapper3(&subject, &Subject::capitalize_words, { 
        {"text", ""} 
    });
    Wrapper wrapper4(&subject, &Subject::capitalize_sentences, { 
        {"text", ""} 
    });
    Wrapper wrapper5(&subject, &Subject::add_sentence_numbers, { 
        {"text", ""} 
    });

    Engine engine;

    engine.register_command(&wrapper1, "command1");
    engine.register_command(&wrapper2, "command2");
    engine.register_command(&wrapper3, "command3");
    engine.register_command(&wrapper4, "command4");
    engine.register_command(&wrapper5, "command5");

    std::string file_path;
    std::cout << "Enter the file path: ";
    std::cin >> file_path;

    std::string file_content = read_file(file_path);
    if (file_content.empty()) return 0;

    bool continue_program = true;
    while (continue_program) {
        std::cout << "Choose a command (1-5):\n";
        std::cout << "1. Replace word\n2. Search phrase\n3. Capitalize words\n4. Capitalize sentences\n5. Add sentence numbers\n6. Exit\n";
        int command_choice;
        std::cin >> command_choice;

        ArgsMap args_map;
        switch (command_choice) {
            case 1: {
                std::string old_word, new_word;
                std::cout << "Enter the word to replace: ";
                std::cin >> old_word;
                std::cout << "Enter the new word: ";
                std::cin >> new_word;
                args_map = {{"text", file_content}, {"old_word", old_word}, {"new_word", new_word}};
                break;
            }
            case 2: {
                std::string phrase;
                std::cout << "Enter the phrase to search: ";
                std::cin >> phrase;
                args_map = {{"text", file_content}, {"phrase", phrase}};
                break;
            }
            case 3:
                args_map = {{"text", file_content}};
                break;
            case 4:
                args_map = {{"text", file_content}};
                break;
            case 5:
                args_map = {{"text", file_content}};
                break;
            case 6:
                std::cout << "Exiting program...\n";
                continue_program = false;
                break;
            default:
                std::cerr << "Invalid command choice" << std::endl;
                continue;
        }

        if (command_choice != 6) {
            std::string result = engine.execute("command" + std::to_string(command_choice), args_map);
            std::cout << "Result: \n" << result << std::endl;

            char write_choice;
            std::cout << "Do you want to save the result back to the file? (y/n): ";
            std::cin >> write_choice;
            if (write_choice == 'y' || write_choice == 'Y') {
                write_file(file_path, result);
                std::cout << "Result saved to file." << std::endl;
            }
        }
    }

    return 0;
}

cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)

project("calculator")

# Директория для выходных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)

# Добавляем исполняемые файлы
add_executable(calculator "main.cpp" "calculator.h" "calculator.cpp" "loader.h" "loader.cpp" "token.h" "token.cpp" "tokenizer.h" "tokenizer.cpp")

# Указываем путь к заголовочным файлам
target_include_directories(calculator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Указываем, что будем использовать Windows API
add_custom_target(NewPlugins ALL)

# Копируем plugins в конфигурационную папку
add_custom_command(
    TARGET NewPlugins
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/plugins ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins
)

add_dependencies(calculator NewPlugins)
#include <iostream>
#include <valarray>
#include <sstream>
#include <fstream>

std::string read_file(char* filename) {
    std::stringstream code_buffer;
    std::ifstream code_input_file(filename);
    code_buffer << code_input_file.rdbuf();

    std::string resulting_string = code_buffer.str();
    return resulting_string;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./vernam.exe <secret_code_file>" << std::endl;
        return 1;
    }

    std::string resulting_string = read_file(argv[1]);
    size_t code_length = resulting_string.length();
    std::valarray<char> secret_code(code_length);
    for (int i = 0; i < code_length; ++i) {
        secret_code[i] = resulting_string[i];
    }
    std::cout << "Secret code initialized. You can code/decode " << code_length << " characters" << std::endl;

    std::string input_buffer;
    size_t coded = 0;
    while (!std::cin.eof() && coded < code_length) {
        std::getline(std::cin, input_buffer);
        std::valarray<char> read_chunk(input_buffer.length());
        for (int i = 0; i < input_buffer.length(); ++i) {
            read_chunk[i] = input_buffer[i];
        }

        std::valarray<char> sliced = secret_code[std::slice(coded, std::min(coded + input_buffer.size(), code_length), 1)];
        auto resulted_chunk = read_chunk ^ sliced;
        for (int i = 0; i < std::min(resulted_chunk.size(), code_length - coded); ++i) {
            std::cout << resulted_chunk[i];
        }
        std::cout << std::endl;
        coded += input_buffer.length();
        if (coded < code_length) {
            std::cout << code_length - coded << " symbols to code/decode" << std::endl;
        }
    }

    return 0;
}

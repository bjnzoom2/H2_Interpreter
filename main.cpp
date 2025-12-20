#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "tokenization.h"

void interpret(const std::vector<Token> &tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];
        if (token.tokenType == TokenType::UNKNOWN) {
            std::cerr << "Unknown syntax detected\n";
            return;
        }
        if (token.tokenType == TokenType::RETURN && i < tokens.size()-2) {
            if (tokens[i+1].tokenType == TokenType::NUMBER && tokens[i+2].tokenType == TokenType::SEMICOLON) {
                std::cout << tokens[i+1].text << '\n';
            }
        }
    }
}

std::string tokensToASM(const std::vector<Token> &tokens) {
    std::stringstream output;
    output << "global main\nsection .text\nmain:\n";
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];
        if (token.tokenType == TokenType::RETURN && i < tokens.size()-2) {
            if (tokens[i+1].tokenType == TokenType::NUMBER && tokens[i+2].tokenType == TokenType::SEMICOLON) {
                output << "    mov eax, " << std::stoi(tokens[i+1].text) << "\n";
                output << "    ret\n";
            }
        }
    }

    return output.str();
}

int main() {
    std::ifstream codeFile{"File.h2"};

    if (codeFile.is_open()) {
        std::string inputLine;
        std::stringstream inputLineStream;
        inputLineStream << codeFile.rdbuf();
        inputLine = inputLineStream.str();
        std::string asmCode;

        Tokenizer tokenizer(std::move(inputLine));
        std::vector<Token> tokens = tokenizer.tokenise();
        //interpret(tokens);
        asmCode = tokensToASM(tokens);

        std::ofstream asmFile{"out.asm"};
        if (asmFile.is_open()) {
            asmFile << asmCode;
            std::cout << "Successfully Compiled";
        } else {
            std::cout << "Failed to create file\n";
        }
    } else {
        std::cerr << "Failed to open file\n";
    }

    return 0;
}
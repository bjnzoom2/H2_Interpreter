#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "tokenization.h"
#include "parsing.h"
#include "generation.h"

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
        Parser parser(std::move(tokens));
        std::optional<node::RetNode> tree = parser.parseReturn();
        if (!tree.has_value()) {
            std::cerr << "Parsing Failed\n";
            exit(EXIT_FAILURE);
        }
        Generator generator(std::move(tree.value()));
        asmCode = generator.generate();

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
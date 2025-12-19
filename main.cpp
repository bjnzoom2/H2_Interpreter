#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>

const std::string ret = "return";

enum TokenType {UNKNOWN, SEMICOLON, RETURN, NUMBER, IDENTIFIER, ASSIGN};

struct Token {
    TokenType tokenType = UNKNOWN;
    std::string text;
};

std::unordered_map<std::string, int> symbolTable;

std::vector<Token> tokenise(const std::string& input) {
    std::vector<Token> tokens;
    int i = 0;
    while (i < input.size()) {
        if (std::isspace(input[i])) {
            i++;
            continue;
        }

        if (input.substr(i, ret.size()) == ret && (i + ret.size() == input.size() || std::isspace(input[i + ret.size()]))) {
            tokens.push_back({TokenType::RETURN, ret});
            i += ret.size();
            continue;
        }

        if (std::isdigit(input[i])) {
            std::string num;
            while (i < input.size() && std::isdigit(input[i])) {
                num += input[i];
                i++;
            }
            tokens.push_back({TokenType::NUMBER, num});
            continue;
        }

        if (std::isalpha(input[i])) {
            std::string word;
            while (i < input.size() && std::isalpha(input[i])) {
                word += input[i];
                i++;
            }
            tokens.push_back({TokenType::IDENTIFIER, word});
            continue;
        }

        if (input[i] == '=') {
            tokens.push_back({TokenType::ASSIGN, "="});
            i++;
            continue;
        }

        if (input[i] == ';') {
            tokens.push_back({TokenType::SEMICOLON, ";"});
            i++;
            continue;
        }

        tokens.push_back({TokenType::UNKNOWN, std::string(1, input[i])});
        i++;
    }

    return tokens;
}

std::string tokensToASM(std::vector<Token> tokens) {
    std::stringstream output;
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens[i];
        if (token.tokenType == TokenType::RETURN && i < tokens.size()-2) {
            if (tokens[i+1].tokenType == TokenType::NUMBER && tokens[i+2].tokenType == TokenType::SEMICOLON) {

            }
        }
    }

    return output.str();
}

int main() {
    std::ifstream codeFile{"File.h2"};

    if (codeFile.is_open()) {
        std::string inputLine;
        while (std::getline(codeFile, inputLine)) {
            std::vector<Token> tokens = tokenise(inputLine);
            for (int i = 0; i < tokens.size(); i++) {
                Token token = tokens[i];
                if (token.tokenType == TokenType::UNKNOWN) {
                    std::cerr << "Unknown syntax detected\n";
                    return -1;
                }
                if (token.tokenType == TokenType::RETURN && i < tokens.size()-2) {
                    if (tokens[i+1].tokenType == TokenType::NUMBER && tokens[i+2].tokenType == TokenType::SEMICOLON) {
                        std::cout << tokens[i+1].text << '\n';
                    } else if (tokens[i+1].tokenType == TokenType::IDENTIFIER) {
                        if (symbolTable[tokens[i+1].text]) std::cout << symbolTable[tokens[i+1].text] << '\n';
                    }
                }
                if (token.tokenType == TokenType::ASSIGN) {
                    if (tokens[i-1].tokenType == TokenType::IDENTIFIER && tokens[i+1].tokenType == TokenType::NUMBER && tokens[i+2].tokenType == TokenType::SEMICOLON
                        && i > 0 && i < tokens.size()-2) {
                        symbolTable[tokens[i-1].text] = std::stoi(tokens[i+1].text);
                    }
                }
            }
        }
    } else {
        std::cerr << "Failed to open file\n";
    }

    return 0;
}
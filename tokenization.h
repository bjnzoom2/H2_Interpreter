#pragma once
#include <optional>
#include <vector>
#include <string>

enum TokenType {UNKNOWN, SEMICOLON, RETURN, NUMBER};

struct Token {
    TokenType tokenType = UNKNOWN;
    std::string text;
};

class Tokenizer {
private:
    const std::string m_src;
    size_t m_index = 0;

    [[nodiscard]] std::optional<char> peek(int offset = 0) const {
        if (m_index + offset >= m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index + offset);
        }
    }

    char consume() {
        return m_src.at(m_index++);
    }

public:
    explicit inline Tokenizer(std::string src) : m_src(std::move(src)) {};
    inline std::vector<Token> tokenise() {
        std::vector<Token> tokens;
        std::string buffer;
        while (peek().has_value()) {
            if (std::isalpha(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buffer.push_back(consume());
                }
                if (buffer == "return") {
                    tokens.push_back({TokenType::RETURN, "return"});
                    buffer.clear();
                    continue;
                } else {
                    std::cerr << "Invalid syntax\n";
                    exit(EXIT_FAILURE);
                }
            } else if (std::isdigit(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buffer.push_back(consume());
                }
                tokens.push_back({TokenType::NUMBER, buffer});
                buffer.clear();
                continue;
            } else if (peek().value() == ';') {
                consume();
                tokens.push_back({TokenType::SEMICOLON, ";"});
                continue;
            } else if (std::isspace(peek().value())) {
                consume();
                continue;
            } else {
                std::cerr << "Invalid syntax\n";
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }
};
#pragma once
#include "tokenization.h"

namespace node {
    struct ExprNode {
        Token num_lit;
    };

    struct RetNode {
        ExprNode expr;
    };
}

class Parser {
private:
    const std::vector<Token> m_tokens;
    size_t m_index = 0;

    [[nodiscard]] std::optional<Token> peek(int offset = 0) const {
        if (m_index + offset >= m_tokens.size()) {
            return {};
        } else {
            return m_tokens.at(m_index + offset);
        }
    }

    Token consume() {
        return m_tokens.at(m_index++);
    }
public:
    explicit inline Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {};

    std::optional<node::ExprNode> parseExpr() {
        if (peek().has_value() && peek().value().tokenType == TokenType::NUMBER) {
            return node::ExprNode{consume()};
        } else {
            return {};
        }
    }

    std::optional<node::RetNode> parseReturn() {
        std::optional<node::RetNode> nodeRet;
        while (peek().has_value()) {
            if (peek().value().tokenType == TokenType::RETURN) {
                consume();
                if (auto nodeExpr = parseExpr()) {
                    nodeRet = node::RetNode{nodeExpr.value()};
                } else {
                    std::cerr << "Invalid Expression\n";
                    exit(EXIT_FAILURE);
                }
            }
            if (peek().has_value() && peek().value().tokenType == TokenType::SEMICOLON) {
                consume();
            } else {
                std::cerr << "Invalid Expression\n";
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return nodeRet;
    }
};
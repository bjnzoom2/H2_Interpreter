#pragma once
#include <variant>

#include "tokenization.h"

struct ExprNodeNumLit {
    Token num_lit;
};

struct ExprNodeIdent {
    Token ident;
};

struct ExprNode {
    std::variant<ExprNodeNumLit, ExprNodeIdent> node;
};

struct StmtExitNode {
    ExprNode expr;
};

struct StmtLetNode {
    Token ident;
    ExprNode expr;
};

struct StmtNode {
    std::variant<StmtExitNode, StmtLetNode> stmt;
};

struct ProgNode {
    std::vector<StmtNode> stmtNodes;
};

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

    std::optional<ProgNode> parseProg() {
        ProgNode prog;
        while (peek().has_value()) {
            if (auto stmtNode = parseStmt()) {
                prog.stmtNodes.push_back(stmtNode.value());
            } else {
                std::cerr << "Invalid Stmt\n";
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return prog;
    }

    std::optional<StmtNode> parseStmt() {
        if (peek().has_value() && peek().value().tokenType == TokenType::EXIT && peek(1).has_value() && peek(1).value().tokenType == TokenType::OPEN_PARAN) {
            consume();
            consume();
            StmtExitNode stmtExit;
            if (auto exprNode = parseExpr()) {
                stmtExit = StmtExitNode{exprNode.value()};
            }
            consume();
            if (peek().has_value() && peek().value().tokenType == TokenType::CLOSE_PARAN) {
                consume();
            } else {
                std::cerr << "Close Paran is missing\n";
                exit(EXIT_FAILURE);
            }
            if (peek().has_value() && peek().value().tokenType == TokenType::SEMICOLON) {
                return StmtNode{stmtExit};
            } else {
                std::cerr << "Semicolon is Missing\n";
                exit(EXIT_FAILURE);
            }
        } else if (peek().has_value() && peek().value().tokenType == TokenType::LET &&
            peek(1).has_value() && peek(1).value().tokenType == TokenType::IDENT &&
            peek(2).has_value() && peek(2).value().tokenType == TokenType::EQUAL) {
            consume();
            StmtLetNode stmtLet;
            if (auto exprNode = parseExpr()) {
                stmtLet = StmtLetNode{consume(), exprNode.value()};
            }
            consume();
            consume();
            if (peek().has_value() && peek().value().tokenType == TokenType::SEMICOLON) {
                return StmtNode{stmtLet};
            } else {
                std::cerr << "Semicolon is Missing\n";
                exit(EXIT_FAILURE);
            }
        } else {
            return {};
        }
    }

    std::optional<ExprNode> parseExpr() {
        if (peek().has_value() && peek().value().tokenType == TokenType::NUM_LIT) {
            return ExprNode{ExprNodeNumLit{consume()}};
        } else if (peek().has_value() && peek().value().tokenType == TokenType::IDENT) {
            return ExprNode{ExprNodeIdent{peek().value()}};
        } else {
            return {};
        }
    }
};
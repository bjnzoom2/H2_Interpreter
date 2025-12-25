#pragma once
#include <sstream>
#include "parsing.h"

class Generator {
private:
    ProgNode m_tree;
    static std::stringstream m_output;
public:
    explicit inline Generator(ProgNode tree) : m_tree(std::move(tree)) {}
    void gen_expr(ExprNode &expr) {
        struct expr_Visitor {
            void operator() (ExprNodeNumLit& exprNumLit) {
                //TBA
            };

            void operator() (ExprNodeIdent& exprIdent) {
                //TBA
            };
        };
        std::visit(expr_Visitor(), expr.node);
    }

    void gen_stmt(StmtNode &stmt) {
        struct stmt_Visitor {
            void operator() (StmtExitNode& stmtExit) {
                //TBA
            };

            void operator() (StmtLetNode& stmtLet) {
                //TBA
            };
        };
        std::visit(stmt_Visitor(), stmt.stmt);
    }

    [[nodiscard]] std::string gen_prog() {
        m_output << "global main\nsection .text\nmain:\n";
        for (StmtNode &stmt : m_tree.stmtNodes) {
            gen_stmt(stmt);
        }
        return m_output.str();
    }
};
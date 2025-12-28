#pragma once
#include <sstream>
#include <memory>
#include "parsing.h"

class Generator {
private:
    ProgNode m_tree;
    std::stringstream m_output;
    int m_stacksize = 0;

    void push(const std::string& reg) {
        m_output << "   push " << reg << '\n';
        m_stacksize++;
    }

    void pop(const std::string& reg) {
        m_output << "   pop " << reg << '\n';
        m_stacksize--;
    }
public:
    explicit inline Generator(ProgNode tree) : m_tree(std::move(tree)) {}
    void gen_expr(const ExprNode &expr) {
        struct expr_Visitor {
            Generator* gen;
            void operator() (const ExprNodeNumLit& exprNumLit) const {
                gen->m_output << "   mov rdi, " << std::stoi(exprNumLit.num_lit.text) << '\n';
                gen->push("rdi");
            };

            void operator() (const ExprNodeIdent& exprIdent) const {

            };
        };
        auto visitor = expr_Visitor{.gen = this};
        std::visit(visitor, expr.node);
    }

    void gen_stmt(StmtNode &stmt) {
        struct stmt_Visitor {
            Generator* gen;
            void operator() (const StmtExitNode& stmtExit) const {
                gen->gen_expr(stmtExit.expr);
                gen->pop("rax");
                gen->pop("rbp");
                gen->m_output << "   ret\n";
            };

            void operator() (StmtLetNode& stmtLet) const {

            };
        };
        auto visitor = stmt_Visitor{.gen = this};
        std::visit(visitor, stmt.stmt);
    }

    [[nodiscard]] std::string gen_prog() {
        m_output << "global main\nsection .text\nmain:\n   push rbp\n   mov rbp, rsp\n";
        for (StmtNode &stmt : m_tree.stmtNodes) {
            gen_stmt(stmt);
        }
        return m_output.str();
    }
};
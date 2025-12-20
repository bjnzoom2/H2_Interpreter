#pragma once
#include <sstream>
#include "parsing.h"

class Generator {
private:
    node::RetNode m_tree;
public:
    explicit inline Generator(node::RetNode tree) : m_tree(std::move(tree)) {}
    [[nodiscard]] std::string generate() const {
        std::stringstream output;
        output << "global main\nextern printf\nsection .data\nfmt db \"%d\", 10, 0\nsection .text\nmain:\n";
        output << "    sub rsp, 32\n";
        output << "    lea rcx, [rel fmt]\n";
        output << "    mov edx, " << std::stoi(m_tree.expr.num_lit.text) << "\n";
        output << "    xor eax, eax\n";
        output << "    call printf\n";
        output << "    add rsp, 32\n";
        output << "    xor eax, eax\n";
        output << "    ret\n";

        return output.str();
    }
};
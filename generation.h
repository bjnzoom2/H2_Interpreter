#pragma once
#include "parsing.h"

class Generator {
private:
    node::RetNode m_tree;
public:
    explicit inline Generator(node::RetNode tree) : m_tree(std::move(tree)) {}
    [[nodiscard]] std::string generate() const {
        std::stringstream output;
        output << "global main\nsection .text\nmain:\n";
        output << "    mov eax, " << std::stoi(m_tree.expr.num_lit.text) << "\n";
        output << "    ret\n";

        return output.str();
    }
};
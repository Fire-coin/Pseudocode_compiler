#ifndef PSCD_TRANSPILER_AST_HPP
#define PSCD_TRANSPILER_AST_HPP

#include <memory>
#include <string>
#include <vector>

struct visitor {
    void bool_expr();
    void bin_expr();
    void lit_expr();
    void var_expr();
    void decl_stmt();
};

struct ASTNode {
    virtual void visitor();
    virtual ~ASTNode() = default;
};

struct expr : public ASTNode {};

struct stmt : public ASTNode {};

struct bin_expr : public expr {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    
    void visitor();
};

struct bool_expr : public expr {
    bool NOT;
    std::unique_ptr<ASTNode> child; // Child should be bin_expr with logical operator
    void visitor();
};

struct lit_expr : public expr { // Literal value
    std::string val;
    void visitor();
};

struct var_expr : public expr { // Variable
    std::string var;
    void visitor();
};

struct decl_stmt : public stmt {
    std::vector<std::string> idents;
    std::string dtype;
    void visitor();
};

#endif
#pragma once
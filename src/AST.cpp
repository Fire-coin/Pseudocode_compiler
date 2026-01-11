#include "AST.hpp"

void visitor::bool_expr(){}
void visitor::bin_expr(){}
void visitor::lit_expr(){}
void visitor::var_expr(){}
void visitor::decl_stmt(){}


visitor vis;

void ASTNode::visitor() {}

void expr::visitor() {}
void stmt::visitor() {}

void bool_expr::visitor() {
    vis.bool_expr();
}

void bin_expr::visitor() {
    vis.bin_expr();
}

void lit_expr::visitor() {
    vis.lit_expr();
}

void var_expr::visitor() {
    vis.var_expr();
}

void decl_stmt::visitor() {
    vis.decl_stmt();
}

#include "AST.hpp"

visitor vis;

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
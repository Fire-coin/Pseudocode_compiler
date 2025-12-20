#include "parser.hpp"
#include <iostream>

Symbol sym;

Parser::Parser(std::vector<Token> arr): tokens(arr), index(0) {
        this->nextSym();
}

void Parser::nextSym() {
    std::cout << "tokens size: " << this->tokens.size() << " index: " << index << std::endl;
    if (index < this->tokens.size()) {
        sym = lexerToParser(this->tokens[index]);
        this->index++;
        std::cout << "parser sym: " << sym << std::endl;
    }
}

Symbol lexerToParser(Token token) {
    switch (token.tokenName)
    {
    case IDENTIFIER:
        return Symbol::ident;
        break;
    case OPERATOR:
        if (token.tokenValue == "+") 
            return Symbol::plus;
        else if (token.tokenValue == "-")
            return Symbol::minus;
        else if (token.tokenValue == "*")
            return Symbol::star;
        else if (token.tokenValue == "/")
            return Symbol::slash;
        else if (token.tokenValue == "=")   
            return Symbol::eq;
        else if (token.tokenValue == "<>")
            return Symbol::neq;
        else if (token.tokenValue == ">")
            return Symbol::great;
        else if (token.tokenValue == "<")
            return Symbol::less;
        else if (token.tokenValue == ">=")
            return Symbol::greateq;
        else if (token.tokenValue == "<=")
            return lesseq;
        else
            return Symbol::not_token;
        break;
    case SEPARATOR:
        if (token.tokenValue == "(")
            return Symbol::lparen;
        else if (token.tokenValue == ")")
            return Symbol::rparen;
        else
            return Symbol::not_token;
        break;
    case LITERAL:
        if (token.tokenValue.find('\'') == token.tokenValue.npos && token.tokenValue.find('"') == token.tokenValue.npos)
            return Symbol::num_lit;
        break;
    case KEYWORD:
        if (token.tokenValue == "NOT")
            return Symbol::notsym;
        else if (token.tokenValue == "AND")
            return Symbol::andsym;
        else if (token.tokenValue == "OR")
            return Symbol::orsym;
        else
            return Symbol::not_token;
    default:
        return Symbol::not_token;
        break;
    }
}

int Parser::accept(Symbol s) {
    if (s == sym) {
        nextSym();
        return 1;
    }
    return 0;
}
int Parser::expect(Symbol s) {
    if (accept(s))
        return 1;
    std::cout << "Parser: unexpected token: " << tokens[index - 1].tokenValue << std::endl;
    return 0;
}

void Parser::factor() {
    std::cout << "factor called\n";
    if (accept(ident))
        ;
    else if (accept(num_lit)) {
        ;
    }
    else if (accept(lparen)) {
        expr();
        expect(rparen);
    } else {
        std::cout << "Factor: syntax error\n";
        std::cout << sym << std::endl;
        nextSym();
    }
}

void Parser::term() {
    std::cout << "term called\n";
    factor();
    while (sym == star || sym == slash) {
        nextSym();
        factor();
    }
}

void Parser::expr() {
    std::cout << "expr called\n";
    if (sym == plus || sym == minus)
        nextSym();
    term();
    while (sym == plus || sym == minus) {
        nextSym();
        term();
    }
}

/* TODO: Fix this
    Will not accept condition, if it is fully surrounded by parentheses
*/
void Parser::condition() {
    std::cout << "Condition called\n";
    if (accept(notsym))
        ;
    expr();
    if (sym == eq || sym == neq || sym == great || sym == less || sym == greateq || sym == lesseq) {
        nextSym();
        expr();
    } else
        std::cout << "Parser: error unexpected token " << sym << std::endl;
}

void Parser::bool_expr() {
    std::cout << "Bool_expr called\n";
    condition();
    while (sym == andsym || sym == orsym) {
        nextSym();
        condition();
    }
}
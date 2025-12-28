#include "parser.hpp"
#include <iostream>
#include <algorithm>

Symbol sym;
std::vector<std::string> dtypes = {"INTEGER", "REAL", "CHAR", "STRING", "BOOLEAN"};

Parser::Parser(std::vector<Token> arr): tokens(arr), index(0) {
    this->nextSym();
}

int is_dtype(std::string type) {
    std::cout << "Is_dtype called\n";
    for (int i = 0; i < dtypes.size(); ++i) {
        std::cout << dtypes[i] << std::endl;
        if (type == dtypes[i])
            return 1;
    }
    return 0;
}



void Parser::nextSym() {
    // std::cout << "tokens size: " << this->tokens.size() << " index: " << index << std::endl;
    if (index < this->tokens.size()) {
        sym = lexerToParser(this->tokens[index]);
        this->index++;
        // std::cout << "parser sym: " << sym << std::endl;
    }
}

Symbol lexerToParser(Token token) {
    switch (token.tokenName) {
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
            return Symbol::lesseq;
        else if (token.tokenValue == "<--")
            return Symbol::assignsym;
        else
            return Symbol::not_token;
        break;
    case SEPARATOR:
        if (token.tokenValue == "(")
            return Symbol::lparen;
        else if (token.tokenValue == ")")
            return Symbol::rparen;
        else if (token.tokenValue == ",")
            return Symbol::comma;
        else if (token.tokenValue == ":")
            return Symbol::colon;
        
        
        else
            return Symbol::not_token;
        break;
    case LITERAL:
        if (token.tokenValue.find('\'') == token.tokenValue.npos && token.tokenValue.find('"') == token.tokenValue.npos)
            return Symbol::num_lit;
        else
            return Symbol::not_token;
        break;
    case KEYWORD:
        if (token.tokenValue == "NOT")
            return Symbol::notsym;
        else if (token.tokenValue == "AND")
            return Symbol::andsym;
        else if (token.tokenValue == "OR")
            return Symbol::orsym;
        else if (token.tokenValue == "DECLARE")
            return Symbol::declsym;
        else if (token.tokenValue == "INPUT")
            return Symbol::inputsym;
        else if (token.tokenValue == "IF")
            return Symbol::ifsym;
        else if (token.tokenValue == "THEN")
            return Symbol::thensym;
        else if (token.tokenValue == "ENDIF")
            return Symbol::eifsym;
        else if (token.tokenValue == "ELSE")
            return Symbol::elsesym;
        else if (token.tokenValue == "WHILE")
            return Symbol::whilesym;
        else if (token.tokenValue == "ENDWHILE")
            return Symbol::ewhilesym;
        else if (token.tokenValue == "TRUE")
            return Symbol::truesym;
        else if (token.tokenValue == "FALSE")
            return Symbol::falsesym;
        else if (is_dtype(token.tokenValue))
            return Symbol::dtype;
        else
            return Symbol::not_token;
    case WHITESPACE:
        if (token.tokenValue == "\n")
            return Symbol::eol;
        else if (token.tokenValue == "<tab>")
            return Symbol::indent;
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

std::unique_ptr<expr> Parser::parseFactor() {
    std::cout << "factor called\n";
    if (accept(ident)) {
        std::unique_ptr<var_expr> p = std::make_unique<var_expr>();
        p->var = tokens[index - 1].tokenValue;
        return p;
    }
    else if (accept(num_lit)) {
        std::unique_ptr<lit_expr> p = std::make_unique<lit_expr>();
        p->val = tokens[index - 1].tokenValue;
        return p;
    }
    else if (accept(lparen)) {
        std::unique_ptr<expr> p = parseExpr();
        expect(rparen);
        return p;
    } else {
        std::cout << "Factor: syntax error\n";
        std::cout << sym << std::endl;
        nextSym();
    }
}

std::unique_ptr<expr> Parser::term() {
    std::cout << "term called\n";
    std::unique_ptr<expr> p1 = parseFactor();
    if (sym != star && sym != slash)
        return p1;
    
    std::unique_ptr<bin_expr> root = std::make_unique<bin_expr>();
    root->op = tokens[index - 1].tokenValue;
    nextSym();
    std::unique_ptr<expr> p2 = parseFactor();

    root->left = std::move(p1);
    root->right = std::move(p2);
    /* TODO finish the binary node */
    while (sym == star || sym == slash) {
        nextSym();
        p1 = parseFactor();

    }
}

std::unique_ptr<expr> Parser::parseExpr() {
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
    Will not accept parseCondition, if it is fully surrounded by parentheses
*/
void Parser::parseCondition() {
    std::cout << "parseCondition called\n";
    if (accept(notsym))
        ;
    if (sym == truesym || sym == falsesym)
        nextSym();
    else
        parseExpr();
    if (sym == eq || sym == neq || sym == great || sym == less || sym == greateq || sym == lesseq) {
        nextSym();
        if (sym == truesym || sym == falsesym)
            nextSym();
        else
            parseExpr();
    } else
        std::cout << "parseCondition: error unexpected token " << sym << std::endl;
}

void Parser::parseBoolExpr() {
    std::cout << "parseBoolExpr called\n";
    parseCondition();
    while (sym == andsym || sym == orsym) {
        nextSym();
        parseCondition();
    }
}

int Parser::parseStatement() {
    std::cout << "parseStatement called\n";
    /* Consuming the indentation before parseStatements, if they are inside of parseBlock */
    while (accept(indent))
        ;
    if (accept(declsym)) { // Declaration of vaeriable
        expect(ident);
        while (accept(comma))
            expect(ident);
        expect(colon);
        expect(dtype);
        expect(eol);
    } else if (accept(ident)) { // Assignment of variable
        expect(assignsym);
        parseExpr();
        expect(eol);
    } else if (accept(inputsym)) { // Input parseStatement
        expect(ident);
        expect(eol);
    } else if (accept(ifsym)) { // If parseStatement
        parseBoolExpr();
        if (accept(eol))
            ;
        expect(thensym); // Body of if parseStatement
        expect(eol);
        parseBlock();

        if (accept(elsesym)) { // Optional else parseStatement
            expect(eol);            
            parseBlock();
        } 
        expect(eifsym); // End of if parseStatements
        expect(eol);
    } else if (accept(whilesym)) { // While parseStatement
        parseBoolExpr();
        expect(eol);
        parseBlock();
        expect(ewhilesym);
        expect(eol);
    } else {
        std::cout << "parseStatement: error unexpected token: " << sym << std::endl;
        return 0;
    }
    return 1;
}

void Parser::parseBlock() {
    std::cout << "parseBlock called\n";
    while (sym != eifsym && sym != ewhilesym && sym != elsesym) {
        if (!parseStatement()) 
            break;
        while (accept(indent))
            ;
        std::cout << "parseBlock: sym is "<< sym << std::endl;
    }
}
#include "CompilerKit/Parser.hpp"
#include "CompilerKit/Sema.hpp"
#include "PALSema.h"
using namespace CompilerKit;

class PALParser : public Parser {
public:
	PALParser(Scanner& scanner) :
		Parser(scanner),
		semantics(*this) {}

private:
	PALSema semantics;

	void recStarter() override{
		Scope::open();
		expect("PROGRAM");
		expect(Token::Identifier);
		expect("WITH");
		recVarDecls();
		expect("IN");	
		while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have(Token::Integer))
		{
			recStatement();
		}	
		expect("END");
		Scope::close();
	}

	void recVarDecls() {

		while (have(Token::Identifier))
		{
			recIdentList();
			expect("AS");
			recType();
		}
	}

	void recType() {
		if (have("REAL"))
		{
			expect("REAL");
		}
		else if (have("INTEGER")) {
			expect("INTEGER");
		}
		else {
			syntaxError("INTEGER or REAL");
		}
	}

	void recStatement() {
		if (have(Token::Identifier))
		{
			recAssignment();
		}
		else if (have("UNTIL"))
		{
			recLoop();
		}
		else if (have("IF"))
		{
			recConditional();
		}
		else if (have("INPUT"))
		{
			recIO();
		}
		else
		{
			syntaxError("valid statement");
		}

	}

	void recAssignment() {
		expect(Token::Identifier);
		expect("=");
		recExpression();
	}

	void recLoop() {
		expect("UNTIL");
		recBooleanExpr();
		expect("REPEAT");

		while (have(Token::Identifier)|| have("UNTIL")|| have("IF")|| have("INPUT"))
		{
			recStatement();
		}
		expect("ENDLOOP");
	}

	void recConditional() {
		expect("IF");
		recBooleanExpr();
		expect("THEN");

		while (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT"))
		{
			recStatement();
		}


	}

	void recIO() {

	}

	void recExpression() {
		recTerm();

		while (have("+") || have("-"))
		{
			recTerm();
		}
	}

	void recTerm() {
		recFactor();

		while (have("*") || have("/"))
		{
			recFactor();
		}
	}

	void recFactor() {

		if (have("+"))
		{
			expect("+");
		}
		else if (have("-"))
		{
			expect("-");
		}

		if (have(Token::Identifier)||have(Token::Integer) || have(Token::Real))
		{
			recValue();
		}
		else if (have("("))
		{
			expect("(");
			recExpression();
			expect(")");
		}
		
	}

	void recValue() {
		if (have(Token::Identifier))
		{
			expect(Token::Identifier);
		}
		else if (have(Token::Integer))
		{
			expect(Token::Integer);
		}
		else if (have(Token::Real))
		{
			expect(Token::Real);
		}

	}

	void recBooleanExpr() {

	}

	void recIdentList() {
		expect(Token::Identifier);

		while (have(","))
		{
			expect(",");
			expect(Token::Identifier);
		}
	}

	void recDecl() {
		Type type = Type::Invalid;
		if (match("int")) {
			type = Type::Integer;
		}
		else if (match("real")) {
			type = Type::Real;
		}
		//sema.define(scanner().current(), type);
		expect(Token::Identifier);
	}


};

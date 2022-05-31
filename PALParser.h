#include "CompilerKit/Parser.hpp"
#include "CompilerKit/Sema.hpp"
#include "PALSema.h"
using namespace CompilerKit;
#include <string>
#include <iostream>

class PALParser : public Parser {
public:
	PALParser(Scanner& scanner) : 
		Parser(scanner)
		,semantics(*this) {}

private:
	PALSema semantics;
	std::vector<Token> tokenVector;
	
	void recStarter() override{
		Scope::open();
		expect("PROGRAM");
		expect(Token::Identifier);
		expect("WITH");
		recVarDecls();
		expect("IN");	
		while (statementCheck())
		{
			recStatement();
		}	
		expect("END");
		Scope::close();
	}

	
	// <VarDecls> ::= (<IdentList> AS <Type>)* ;
	void recVarDecls() {
		Type type;
		while (have(Token::Identifier))
		{
			tokenVector = recIdentList();
			expect("AS");
			type = recType();

			//traverse vector
			for (Token t : tokenVector)
			{
				semantics.declareVar(t, type);
			}
			
		}
	}

	// <Type> ::= REAL | INTEGER ;
	Type recType() {
		if (have("REAL"))
		{
			expect("REAL");
			return Type::Real;
		}
		else if (have("INTEGER")) {
			expect("INTEGER");
			return Type::Integer;
		}
		else {
			syntaxError("INTEGER or REAL");
			
		}


	}

	// <Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;
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
		else if (have("INPUT")||have("OUTPUT"))
		{
			recIO();
		}
		else
		{
			syntaxError("valid statement");
		}

	}

	// <Assignment> ::= Identifier = <Expression> ;
	void recAssignment() {
		Token varToken = current();
		expect(Token::Identifier);
		expect("=");
		Type exType = recExpression();
		semantics.checkAssignment(varToken, exType);
	}

	// <Loop> ::= UNTIL <BooleanExpr> REPEAT (<Statement>)* ENDLOOP ;
	void recLoop() {
		expect("UNTIL");
		recBooleanExpr();
		expect("REPEAT");

		while (statementCheck())
		{
			recStatement();
		}
		expect("ENDLOOP");
	}


	/*
	* <Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
                      ( ELSE (<Statement>)* )? 
                      ENDIF ;
	*/
	void recConditional() {
		expect("IF");
		recBooleanExpr();
		expect("THEN");

		while (statementCheck())
		{
			recStatement();
		}
		if (match("ELSE"))
		{
			while (statementCheck())
			{
				recStatement();
			}
		}
		expect("ENDIF");

	}


	/*
	* <I-o> ::= INPUT <IdentList> | 
          OUTPUT <Expression> ( , <Expression>)* ;
	*/
	void recIO() {
		if (have("INPUT"))
		{
			expect("INPUT");
			recIdentList();
		}
		else if (have("OUTPUT"))
		{
			expect("OUTPUT");
			recExpression();

			while (match(","))
			{
				recExpression();
			}
		}
	}

	// <Expression> ::= <Term> ( (+|-) <Term>)* ;
	Type recExpression() {
		Type leftType = recTerm();

		Token op = current();
		while (have("+") || have("-"))
		{
			if (match("+")) {

			}
			else if (match("-")) {

			}
			Type rightType = recTerm();
			//checking both term values match
			leftType = semantics.checkExpression(op, leftType,  rightType);

			op = current();
		}
		return leftType;
	}

	// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
	Type recTerm() {
		Type leftType = recFactor();
		Token op = current();
		while (have("*") || have("/"))
		{
			if (match("*")) {

			}
			else if (match("/")) {

			}
			Type rightType = recFactor();
			
			//checking both factor values match
			leftType = semantics.checkExpression(op, leftType, rightType);

			op = current();
		}
		return leftType;
	}

	// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
	Type recFactor() {

		if (have("+"))
		{
			expect("+");
		}
		else if (have("-"))
		{
			expect("-");
		}

		Type type;

		if (have(Token::Identifier)||have(Token::Integer) || have(Token::Real))
		{
			type = recValue();
		}
		else if (have("("))
		{
			expect("(");
			type = recExpression();
			expect(")");
		}
		else {
			syntaxError("Valid Statement");
		}
		return type;
	}

	// <Value> ::= Identifier | IntegerValue | RealValue ;
	Type recValue() {
		Type type;
		if (have(Token::Identifier))
		{
			type = semantics.checkID(current());
			expect(Token::Identifier);
		}
		else if (have(Token::Integer))
		{
			expect(Token::Integer);
			type = Type::Integer;
		}
		else if (have(Token::Real))
		{
			expect(Token::Real);
			type = Type::Real;
		}
		else
		{
			syntaxError("Identifier or Integer or Real");
		}
		return type;

	}

	// <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
	void recBooleanExpr() {
		Type leftType = recExpression();
		Token op = current();
		if (have("<"))
		{
			expect("<");
		}
		else if (have("="))
		{
			expect("=");
		}
		else if (have(">"))
		{
			expect(">");
		}
		Type rightType = recExpression();

		semantics.checkExpression(op, leftType, rightType);
	}

	// <IdentList> ::= Identifier ( , Identifier)* ;
	std::vector<Token> recIdentList() {
		std::vector<Token> tokens;
		tokens.push_back(current());
		expect(Token::Identifier);
		while (have(","))
		{
			expect(",");
			tokens.push_back(current());
			expect(Token::Identifier);
		}
		return tokens;
	}

	bool statementCheck() {
		if (have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT"))
		{
			return true;
		}
		else {
			return false;
		}
	}

};

#include "CompilerKit/Sema.hpp"
using namespace CompilerKit;

class PALSema : public Sema {
public:

	PALSema(Parser& parser) : Sema(parser) {
		
	}
	
	//Declares variable if not already defined
	void declareVar(const Token& token, Type type) {
		if (!token.is(Token::Identifier)) return;

		Scope *scope = Scope::current();

		if (scope->isDefined(token.text()))
		{
			redeclaredError(token);
		}
		else { 
			scope->define(token, type);
		}
	}

	// returns identifier type
	Type checkID(Token token) {
		if (!token.is(Token::Identifier)) return Type::Invalid;
		if (checkDefined(token))
		{
			return Scope::current()->get(token.text())->type;
		}
		else {
			return Type::Invalid;
		}
		
	}

	// Checks variable has been defined before use
	bool checkDefined(Token token) {
		if (!Scope::current()->isDefined(token.text()))
		{
			undeclaredError(token);
			return false;
		}
		else {
			return true;
		}
		
	}

	// checks both types are compatible
	Type checkExpression(Token token,Type type1, Type type2) {
		if (type1 != type2)
		{
			binaryExprError(token, type1, type2);
			return Type::Invalid;
		}
		return type1;
	}

	// checks variables are defined with compatible types
	void checkAssignment(Token token, Type type) {
		Type varType = checkID(token);
		if (varType == Type::Invalid) return;
		if (varType != type)
		{
			assignmentError(token, Scope::current()->get(token.text())->type, type);
		}
	}

};

#include "CompilerKit/Parser.hpp"
using namespace CompilerKit;

class PALParser : public Parser {
public:
	PALParser(Scanner& scanner) : Parser(scanner) {}
private:
	void recStarter() override{

		while (have("int") || have("real")) {
			recDeclaration();
		}

		recBlock();
	}

	void recDeclaration() {
		if (have("int"))
		{
			expect("int");
		}
		else {
			expect("real");
		}
		expect(Token::Identifier);

		while (match(","))
		{
			expect(Token::Identifier);
		}

	}

	void recBlock() {
		if (have("let")||have("for") || have("get") || have("put"))
		{
			recStatement();
		}
		else 
		{
			expect("begin");
			
			while (have("let") || have("for") || have("get") || have("put"))
			{
				recStatement();
			}

			expect("end");
		}
	}

	void recStatement()
	{
		if (have("let"))
		{
			recLetStatement();
		}
		else if ("for")
		{
			recForStatement();
		}
	}

	void recLetStatement() 
	{
		expect("let");
		expect(Token::Identifier);
		expect(":=");
		recExpression();
	}
	void recForStatement() 
	{
		expect("for");
		expect(Token::Identifier);
		expect(":=");
		recExpression();
		expect("to");
		recExpression();
		expect("do");
		recBlock();
	}

	void recExpression() 
	{
		recTerm();

		while (have("+")||have("-"))
		{
			if (have("+"))
			{
				expect("+");
			}
			else
			{
				expect("-");
			}
			recTerm();
		}
	}

	void recTerm() 
	{
		recFactor();
		
		while (have("*") || have("/"))
		{
			if (have("*"))
			{
				expect("*");
			}
			else
			{
				expect("/");
			}

			recFactor();
		}
	}

	void recFactor() 
	{
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
		else
		{
			expect("(");
			recExpression();
			expect(")");
		}
	}

};

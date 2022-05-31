#pragma once
// import the Scanner module
#include <sstream>
#include <iostream>
#include <string>
#include <set>

#include "CompilerKit/Scanner.hpp"
using namespace CompilerKit;

// Define a subclass
class PALScanner : public Scanner {
public:
	std::string keywords[16] = { "PROGRAM", "WITH", "IN", "END", "AS", "INTEGER", "REAL", "UNTIL","REPEAT","ENDLOOP","IF","THEN","ELSE","ENDIF","INPUT","OUTPUT"};

	enum class State {
		Start = 0,
		Identifier = 1,
		Integer = 2,
		Real = 3,
		Punct = 4,
		EndOfFile = 98,
		InvalidChar = 99,
	};

	PALScanner(std::istream& in) : Scanner(in) {}
	virtual ~PALScanner() {}

	// Implement lex()
	Token lex() {

		State state = State::Start;
		std::stringstream buf;

		while (true) {
			switch (state) {
			case State::Start:     // start state.
				if (std::isspace(peek())) {
					state = State::Start;
				}
				else {
					updateTokenStart();
					if (std::isalpha(peek()))				state = State::Identifier;
					else if (std::isdigit(peek()))			state = State::Integer;
					else if (peek() == '\0')				state = State::EndOfFile;
					else if (strchr("+-*/(),=<>", peek()))	state = State::Punct;
					else									state = State::InvalidChar;
				}
				break;

			case State::Identifier:     // Identifier
				if (std::isalnum(peek())) {
					state = State::Identifier;
				}
				else {
					std::string str = buf.str();
					for (size_t i = 0; i < 16; i++)
					{
						if (str == keywords[i]) {
							return makeToken(str);
						}
					}
					return makeToken(Token::Identifier, str);
				}
				break;

			case State::Integer:     // Integer
				if (std::isdigit(peek())) {
					state = State::Integer;
				}
				else if (peek() == '.')
				{
					state = State::Real;
				}
				else {
					return makeToken(Token::Integer, buf.str());
				}
				break;

			case State::Real:     // Real
				if (std::isdigit(peek())) {
					state = State::Real;
				}
				else {
					return makeToken(Token::Real, buf.str());
				}
				break;

			case State::Punct:			// punctuation
				return makeToken(buf.str());

			case State::EndOfFile:     // end-of-file
				return makeToken(Token::EndOfFile);

			case State::InvalidChar:     // invalid-char
				return makeToken(Token::InvalidChar, buf.str());
			}

			if (state != State::Start)
			{
				buf << peek();
			}
			advance();
		}
		return makeToken(Token::InvalidToken);
	}
};



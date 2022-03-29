# PALCompiler
Compiler for PAL (Pretty Awful Language)

Uses [CompilerKit](https://amyparent.com/CompilerKit/) for recursive descent parsing

## PAL EBNF

```
<Program> ::= PROGRAM Identifier
              WITH <VarDecls> 
              IN (<Statement>)+
              END ;
              
<VarDecls> ::= (<IdentList> AS <Type>)* ;

<Type> ::= REAL | INTEGER ;

<Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;

<Assignment> ::= Identifier = <Expression> ;

<Loop> ::= UNTIL <BooleanExpr> REPEAT (<Statement>)* ENDLOOP ;

<Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
                      ( ELSE (<Statement>)* )? 
                      ENDIF ;

<I-o> ::= INPUT <IdentList> | 
          OUTPUT <Expression> ( , <Expression>)* ;

<Expression> ::= <Term> ( (+|-) <Term>)* ;

<Term> ::= <Factor> ( (*|/) <Factor>)* ;

<Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;

<Value> ::= Identifier | IntegerValue | RealValue ;

<BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;

<IdentList> ::= Identifier ( , Identifier)* ;

microsyntax 

Identifier   <|[a-zA-Z]\w*
RealValue    <|\d+\.\d*
IntegerValue <|\d+
```

#include <string>
class Scope
{
public:
	static Scope* open();
	static Scope* current();
	static void close();

	/*bool define(Token token, Type type);
	bool isDefined(string name) const;
	const Symbol* get(string name) const;
	*/
	};

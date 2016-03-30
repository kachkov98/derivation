#include <stdlib.h>
#include "tree.h"

#define C(x) Node(x)

Tree expression  (C(2.0) + C(3.0) * C('x'));

int main (int argc, char *argv[])
{
	expression.Print ();
	expression.PrintExpression ();
	
	Tree diff_expression = expression.Diff ('x');
	diff_expression.Print ();
	diff_expression.PrintExpression ();
	system ("pause");
	return 0;
}
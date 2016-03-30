#include <stdio.h>
#include <assert.h>
#include "tree.h"

#define D(x) x->Diff (var)
#define C(x) x->Copy ()
#define ADD(x, y) new Node (OP_PLUS, x, y)
#define SUB(x, y) new Node (OP_MINUS, x, y)
#define MUL(x, y) new Node (OP_MULTIPLY, x, y)
#define DIV(x, y) new Node (OP_DIVIDE, x, y)

//implementation of Node
Node::Node (double x, Node *left_tree, Node *right_tree) :
	type   (NODE_CONSTANT),
	number (x),
	left   (left_tree),
	right  (right_tree)
{
}

Node::Node (char var, Node *left_tree, Node *right_tree) :
	type     (NODE_VARIABLE),
	variable (var),
	left     (left_tree),
	right    (right_tree)
{
}

Node::Node (OPERATION op, Node *left_tree, Node *right_tree) :
	type      (NODE_BIN_OP),
	operation (op),
	left      (left_tree),
	right     (right_tree)
{
}

Node operator+ (Node& l, Node& r)
{
	return Node (OP_PLUS, new Node (l), new Node (r));
}

Node operator- (Node& l, Node& r)
{
	return Node (OP_MINUS, new Node (l), new Node (r));
}

Node operator* (Node& l, Node& r)
{
	return Node (OP_MULTIPLY, new Node (l), new Node (r));
}

Node operator/ (Node& l, Node& r)
{
	return Node (OP_DIVIDE, new Node (l), new Node (r));
}

bool Node::Validate ()
{
	switch (type)
	{
	case NODE_VARIABLE:
	case NODE_CONSTANT:
		if (left || right)
			return false;
		else
			return true;
	case NODE_BIN_OP:
		if (left && right)
			return (left->Validate () && right->Validate ());
		else
			return false;
	}
	return false;
}

void Node::Print (int depth)
{
	printf ("%*s", depth * 4, "");
	switch (type)
	{
	case NODE_CONSTANT:
		printf ("%0.2lf", number);
		break;
	case NODE_VARIABLE:
		printf ("%c", variable);
		break;
	case NODE_BIN_OP:
		switch (operation)
		{
		case OP_PLUS: 
			printf ("+");
			break;
		case OP_MINUS:
			printf ("-");
			break;
		case OP_MULTIPLY:
			printf ("*");
			break;
		case OP_DIVIDE:
			printf ("/");
			break;
		}
		break;
	}
	printf ("\n");
	if (left)
		left->Print (depth + 1);
	if (right)
		right->Print (depth + 1);
}

void Node::PrintExpression ()
{
	switch (type)
	{
	case NODE_BIN_OP:
		printf ("(");
		left->PrintExpression ();
		switch (operation)
		{
		case OP_PLUS:
			printf (") + (");
			break;
		case OP_MINUS:
			printf (") - (");
			break;
		case OP_MULTIPLY:
			printf (") * (");
			break;
		case OP_DIVIDE:
			printf (") / (");
			break;
		}
		right->PrintExpression ();
		printf (")");
		break;
	case NODE_CONSTANT:
		printf ("%0.2lf", number);
		break;
	case NODE_VARIABLE:
		printf ("%c", variable);
		break;
	}
}

Node Node::Diff (char var)
{
	switch (type)
	{
	case NODE_CONSTANT:
		return Node (0.0);
	case NODE_VARIABLE:
		if (variable == var)
			return Node (1.0);
		else
			return Node (var);
	case NODE_BIN_OP:
		#define L left->Copy()
		#define R right->Copy()
		#define dL left->Diff(var)
		#define dR right->Diff(var)
		switch (operation)
		{
		case OP_PLUS:
			return dL + dR;
		case OP_MINUS:
			return dL - dR;
		case OP_MULTIPLY:
			return L*dR + dL*R;
		case OP_DIVIDE:
			return (L*dR - R*dL) / (R*R);
		}
		#undef L
		#undef R
		#undef dL
		#undef dR
	}
	//dead food
	return Node (-1.0);
}

Node Node::Copy ()
{
	switch (type)
	{
	case NODE_CONSTANT:
		return Node (number);
	case NODE_VARIABLE:
		return Node (variable);
	case NODE_BIN_OP:
		return Node (operation, new Node (left->Copy ()), new Node (right->Copy ()));
	}
	//dead food
	return Node (-1.0);
}
//end of section Node


//implementation of Tree
Tree::Tree (Node *root_node) :
	root (root_node)
{
	assert (root);
}

Tree::Tree (Node& root_node)
{
	root = new Node (root_node);
}

Tree::~Tree ()
{
	if (root)
		Erase (root);
}

bool Tree::Validate ()
{
	if (root)
		return root->Validate ();
	else
		return true;
}

void Tree::Erase (Node *node)
{
	if (node->left)
		Erase (node->left);
	if (node->right)
		Erase (node->right);
	delete node;
}

void Tree::Print ()
{
	assert (Validate ());
	if (root)
		root->Print ();
	else
		printf ("Empty tree\n");
}

void Tree::PrintExpression ()
{
	assert (Validate ());
	if (root)
		root->PrintExpression ();
	else
		printf ("Empty expression");
	printf ("\n");
}

Tree Tree::Diff (char variable)
{
	assert (root);
	assert (Validate ());
	return Tree (root->Diff (variable));
}
//end of section Tree

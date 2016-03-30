#pragma once
#include <stddef.h>

#define TERM(x) new Node (x)

enum NODE_TYPE
{
	NODE_CONSTANT,
	NODE_VARIABLE,
	NODE_BIN_OP
};

enum OPERATION
{
	OP_PLUS,
	OP_MINUS,
	OP_MULTIPLY,
	OP_DIVIDE
};

class Node
{
public:
	Node (double x, Node *left_tree = NULL, Node *right_tree = NULL);
	Node (char var, Node *left_tree = NULL, Node *right_tree = NULL);
	Node (OPERATION op, Node *left_tree = NULL, Node *right_tree = NULL);

	friend Node operator+ (Node& l, Node& r);
	friend Node operator- (Node& l, Node& r);
	friend Node operator* (Node& l, Node& r);
	friend Node operator/ (Node& l, Node& r);

	bool Validate ();
	void Print (int depth = 0);
	void PrintExpression ();
	Node Diff (char variable);
	Node Copy ();

	NODE_TYPE type;
	union
	{
		double number;
		char variable;
		OPERATION operation;
	};

	Node *left, *right;
};

class Tree
{
public:
	Tree (Node *root_node);
	Tree (Node &root_node);
	~Tree ();
	bool Validate ();
	void Print ();
	void PrintExpression ();
	Tree Diff (char variable);
private:
	void Erase (Node *node);
	Node *root;
};

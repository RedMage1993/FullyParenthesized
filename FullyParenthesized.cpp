// Fritz Ammon
// Class declaration/definition of linkedStack.
// Stack implemented with linked list.

#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include <iostream>

template <class myType>
struct nodeType
{
	myType info;
	nodeType<myType> *link;
};

template <class myType>
class linkedStack
{
public:
	// Constructors
	linkedStack();
	linkedStack(const linkedStack<myType>&); // Deep copy, calls copy stack
	void initializeStack();	   // Called for constructor

	~linkedStack(); // Destructor
	void deleteStack();		   // Make empty

	bool isStackEmpty() const; // Check if empty

	// Methods to add to stack, peek, and get
	void push(const myType&);
	myType peek() const;
	myType pop();

	int stackCount() const; // Num of nodes in the stack

	bool operator==(const linkedStack<myType>&);
	bool operator!=(const linkedStack<myType>&);

	linkedStack<myType> operator=(const linkedStack<myType>&); // Deep copy, calls copyStack

private:
	void copyStack(const linkedStack<myType>&);
	nodeType<myType> *stackTop;
	int count;
};

template <class myType>
linkedStack<myType>::linkedStack()
{
	initializeStack();
}

template <class myType>
linkedStack<myType>::linkedStack(const linkedStack<myType>& stack)
{
	copyStack(stack);
}

template <class myType>
linkedStack<myType>::~linkedStack()
{
	deleteStack();
}

template <class myType>
void linkedStack<myType>::deleteStack()
{
	nodeType<myType> *node, *tempNode;

	node = stackTop;
	while (node) // while node is not 0
	{
		tempNode = node; // save the node
		node = node->link; // move to next node
		delete tempNode; // delete the saved node
	}

	initializeStack();
}

template <class myType>
bool linkedStack<myType>::isStackEmpty() const
{
	return count == 0;
}

template <class myType>
void linkedStack<myType>::initializeStack()
{
	count = 0;
	stackTop = 0;
}

template <class myType>
void linkedStack<myType>::push(const myType& info)
{
	nodeType<myType> *newTop = new nodeType<myType>;

	newTop->info = info;
	newTop->link = stackTop; // new node points to (last) top

	stackTop = newTop; // new top is teh new node

	count++;
}

template <class myType>
myType linkedStack<myType>::peek() const
{
	if (stackTop)
		return stackTop->info; // just show the info at the top
	else // Know that in the case of double, you return 0.0
	{
		std::cout << "Error (peek), stack is empty.\n";
		return static_cast<myType> (0);
	}
}

template <class myType>
myType linkedStack<myType>::pop()
{
	nodeType<myType> *prevTop;
	myType prevInfo;

	if (stackTop)
	{
		prevTop = stackTop; // Save current top.
		prevInfo = stackTop->info; // Save previous top info.

		stackTop = stackTop->link; // Move to next node (new top).

		delete prevTop; // Delete previous top.

		count--;

		return prevInfo; // Return previous top info.
	}
	else
	{
		std::cout << "Error (pop), cannot pop from an empty stack.\n";
		// Know that in the case of double, you return 0.0
		return static_cast<myType> (0);
	}
}

template <class myType>
int linkedStack<myType>::stackCount() const
{
	return count;
}

template <class myType>
bool linkedStack<myType>::operator==(const linkedStack<myType>& stack)
{
	nodeType<myType> *myNode = stackTop;
	nodeType<myType> *urNode = stack.stackTop;

	if (count != stack.count)
		return false; // shouldn't be different sizes

	while (myNode && urNode)
	{
		if (myNode->info != urNode->info)
			return false;

		myNode = myNode->link;
		urNode = urNode->link;
	}

	return true;
}

template <class myType>
bool linkedStack<myType>::operator!=(const linkedStack<myType>& stack)
{
	return !(*this == stack);
}

template <class myType>
linkedStack<myType> linkedStack<myType>::operator=(const linkedStack<myType>& stack)
{
	copyStack(stack);

	return *this;
}

template <class myType>
void linkedStack<myType>::copyStack(const linkedStack<myType>& stack)
{
	nodeType<myType> *node; // to traverse the passed stack
	nodeType<myType> *newNode, *lastNode; // for this stack

	if (stack.count == 0)
	{
		count = 0;
		stackTop = 0;
		return;
	}

	node = stack.stackTop; // refer to the passed stack's top

	// start off the list
	stackTop = new nodeType<myType>;
	stackTop->info = node->info;
	stackTop->link = 0;
	
	lastNode = stackTop;

	while (node->link != 0) // if true then only one node
	{ // else
		node = node->link; // move down passed stack

		newNode = new nodeType<myType>; // make new node
		newNode->info = node->info; // copy passed info
		newNode->link = 0;
		
		lastNode->link = newNode; // set last node link to new node
		lastNode = newNode; // last is now new node
	}

	count = stack.count;
}

#endif // LINKEDSTACK_H

// Fritz Ammon
// CS 302
// Section 1001
// Homework #04
//
// Description:
// Creates an expression tree and then displays
// the (overly parenthesized) version.
//
// Input: A postfix expression of letter, integer/float
// from stdin or redirection.
//
// Process (as described in book):
// Read expression one symbol at a time.
// If operand, create new tree and push on stack.
// If operator, pop two trees.
// Operator is root and left link becomes second tree popped.
// Right link becomes first pop.
// Then push root onto stack.
//
// Output: The fully parenthesized expression (infix).

#include <iostream>
//#include "linkedStack.h"

using namespace std;

template <class comparable>
struct binaryNode
{
	comparable element;		  // Item
	binaryNode *left, *right; // Children
	bool isChar;			  // To handle multiple data.
	bool isOperator;		  // Makes printing out easier.

	// Constructor for when creating/inserting new node
	binaryNode(const comparable &nElement,  binaryNode<comparable> *nLeft,
		binaryNode<comparable> *nRight, bool op = true, bool ch = true) :
		element(nElement), left(nLeft), right(nRight), isOperator(op), isChar(ch) {}
};

void printInorder(binaryNode<double> const *);
void deleteTree(binaryNode<double> *&);

int main()
{
	binaryNode<double> *tempTree;				// can store double nodes (char too basically)
	linkedStack<binaryNode<double> *> expTree;  // stack holding the subtrees
	char symbol;								// can be operator or variable (a, b, c, ..., z)
	double number;								// can be digit (read in as int or float)

	//cout << "Enter the postfix expression: ";

	while ((symbol = cin.peek()) != '\n' && cin.peek() != -1)
	{
		// Ignore any whitespaces
		while (symbol == ' ' || symbol == '\t')
		{
			cin.get(symbol);
			symbol = cin.peek();
		}

		if ((symbol >= '0' && symbol <= '9') || symbol == '.') // is beginning of number
		{
			// accept anything starting with number 0 to 9 (and . for decimal values)
			cin >> number;
			tempTree = new binaryNode<double> (number, NULL, NULL, false, false);
			expTree.push(tempTree);
		}
		else if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'))
		{
			// accept any variables
			cin >> symbol;
			tempTree = new binaryNode<double> (static_cast<double> (symbol), NULL, NULL, false);
			expTree.push(tempTree);
		}
		else
		{
			cin >> symbol;

			// Check what symbol it was.
			switch (symbol)
			{
			case '+':
			case '-':
			case '*':
			case '/':
				tempTree = new binaryNode<double> (static_cast<double> (symbol), NULL, NULL);
				tempTree->right = expTree.pop();
				tempTree->left  = expTree.pop();
				expTree.push(tempTree);
				break; // Not really needed but oh well.
			default:
				continue;	// Wasn't an operator.
			}
		}
	}

	tempTree = expTree.pop(); // Final pointer on stack should be full expression tree.

	printInorder(tempTree); // Print it as postfix.
	cout << endl << endl;

	deleteTree(tempTree); // Free memory.

	return 0;
}

void printInorder(binaryNode<double> const *t)
// Print the inorder traversal of the tree pointed to by root.
// This will produce/display the parenthesized infix expresion.
{
	if (t != NULL)
	{
		if (t->isOperator)
			cout << "( ";
		printInorder(t->left);

		if (t->isOperator)
			cout << ' ' << static_cast<char> (t->element) << ' ';
		else if (t->isChar)
			cout << static_cast<char> (t->element);
		else
			cout << t->element;

		printInorder(t->right);
		if (t->isOperator)
			cout << " )";
	}
}

void deleteTree(binaryNode<double> *&t)
// Deletes the tree's nodes and frees memory.
{
	if (t != NULL)
	{
		deleteTree(t->left);
		deleteTree(t->right);
		delete t;
	}

	t = NULL;
}

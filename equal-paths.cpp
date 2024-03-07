#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node *root);


bool equalPaths(Node * root)
{
  // Add your code below
  if (root == NULL)
    return true;

  // finding heights of subtrees
	int left = height(root->left);
	int right = height(root->right);

  int difference = 0;
  // if node only has 1 child, no need to compare
  if (left > 0 && right > 0)
    difference = left - right;

	// if the subtrees are not equal in height, return false
	if (difference != 0) {
		return false;
	}

	bool l = true;
	bool r = true;
	// check if subtrees also have subtrees equal in length
	l = equalPaths(root->left);
	r = equalPaths(root->right);

	// if all subtrees have
	return (l == true && r == true);
}

// getting height of a tree
int height(Node *root) {
	if (root == NULL)
		return 0;

	int l = 1;
	int r = 1;
	l += height(root->left);

	r += height(root->right);
	
	if (l > r)
		return l;
	return r;
}

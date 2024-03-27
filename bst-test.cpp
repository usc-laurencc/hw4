#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
  /*
    // Binary Search Tree tests
    
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');

    BinarySearchTree<int, double> bst;
	bst.insert(std::make_pair(2, 1.0));
	bst.insert(std::make_pair(1, 1.0));
	bst.insert(std::make_pair(3, 1.0));
    bst.print();
	bst.remove(2);
    bst.print();
    */
    // AVL Tree Tests
    cout << "STARTING" << endl;
    AVLTree<int, double> bst;
    bst.insert(std::make_pair(2, 1.0));
	bst.insert(std::make_pair(3, 1.0));
	bst.insert(std::make_pair(1, 1.0));
	bst.insert(std::make_pair(4, 1.0));
  bst.print();
	bst.remove(1);
    bst.print();

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<int, double>::iterator it = bst.begin(); it != bst.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    /*
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');
    */
    return 0;
}

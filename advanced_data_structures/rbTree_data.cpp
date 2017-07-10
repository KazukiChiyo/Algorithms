#include <iostream>
#include "rbTree.h"
using namespace std;
int main() {
    rbTree<int, int>* tree = new rbTree<int, int>();
    tree->insert(1, 12500);
    tree->insert(2, 3740);
    tree->insert(3, 4);
    tree->insert(4, 18);
    tree->insert(5, 12);
    tree->insert(6, 120);
    tree->print(tree->getRoot(), 5);
    cout << tree->size() << endl;
    tree->remove(3);
    tree->remove(6);
    tree->remove(1);
    tree->remove(4);
    cout << "##################" << endl;
    tree->print(tree->getRoot(), 5);
}
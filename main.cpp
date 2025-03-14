#include <iostream>
#include "BST.h"

int main() {
    BST<int,double> tree("F:\\Binary Search Tree\\testfile.txt");
    tree.traverse();
    std::cout<<tree.searchKey(4)<<std::endl;
    tree.deleteData(1);
    std::cout<<tree.searchKey(4)<<std::endl;
    tree.deleteData(4);
    std::cout<<tree.searchKey(4)<<std::endl;
}

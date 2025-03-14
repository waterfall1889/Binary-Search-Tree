//
// Created by 30884 on 25-3-14.
//

#ifndef BINARY_SEARCH_TREE_BST_H
#define BINARY_SEARCH_TREE_BST_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <iostream>

template <class KeyType,class ValueType>
class key_value_pair{
public:
    KeyType key;
    ValueType value;
    key_value_pair() = default;
    key_value_pair(const KeyType &k,const ValueType &v):key(k),value(v){}
};


template <class KeyType,class ValueType>
class BST {
private:
    class TreeNode{
    public:
        key_value_pair<KeyType,ValueType> data;
        //in one tree, a key should only appear once at most.
        TreeNode *leftChild;
        TreeNode *rightChild;
        bool isDeleted;

        TreeNode():leftChild(nullptr),rightChild(nullptr),isDeleted(false){}
        TreeNode(const key_value_pair<KeyType,ValueType> &content,
                 TreeNode *left = nullptr,
                 TreeNode *right = nullptr):
                data(content),leftChild(left),rightChild(right),isDeleted(false){}


        key_value_pair<KeyType,ValueType> getData(){return this->data;}

    };

    TreeNode *root;
    void buildFromFile(const std::string &filepath);

public:
    BST():root(nullptr){}
    BST(const std::string &filepath);

    bool isEmpty()const;
    TreeNode *getRoot()const;

    void insertData(TreeNode *&target,const key_value_pair<KeyType,ValueType> &newData);
    void deleteData(const KeyType &k);

    void clear(TreeNode *target);
    void clear();

    int searchKey(const KeyType &key);
    //search the key's rank,if not found then return -1.
    key_value_pair<KeyType,ValueType> searchRank(TreeNode *&target,int64_t rank);
    //search the data at this rank,if above the size or below zero then return an empty pair.
    key_value_pair<KeyType,ValueType> findPreviousItem(const KeyType &key);
    //find the biggest key that smaller than the target.
    key_value_pair<KeyType,ValueType> findFollowingItem(const KeyType &key);
    //find the smallest key that bigger than the target.

    void buildTree(TreeNode *&subRoot,const std::vector<key_value_pair<KeyType,ValueType>> &dataSet);
    void writeDataToVector(TreeNode *&target,std::vector<key_value_pair<KeyType,ValueType>> &dataSet);
    void traverse();
    int getSize(TreeNode *&subRoot)const;
};

template<class KeyType, class ValueType>
int BST<KeyType, ValueType>::getSize(BST::TreeNode *&subRoot) const {
    if(subRoot == nullptr){
        return 0;
    }
    else if(subRoot->isDeleted){
        return getSize(subRoot->leftChild)+ getSize(subRoot->rightChild);
    }
    else if(!subRoot->isDeleted){
        return getSize(subRoot->leftChild)+ getSize(subRoot->rightChild)+1;
    }
}

template<class KeyType, class ValueType>
BST<KeyType, ValueType>::BST(const std::string &filepath) {
    root = nullptr;
    buildFromFile(filepath);
}

template<class KeyType, class ValueType>
bool BST<KeyType, ValueType>::isEmpty() const {
    return (this->root == nullptr);
}

template<class KeyType, class ValueType>
BST<KeyType,ValueType>::TreeNode *BST<KeyType, ValueType>::getRoot() const {
    return this->root;
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::insertData(TreeNode *&target,const key_value_pair<KeyType, ValueType> &newData) {
    //if existed,then replace the original one.
    if(target == nullptr){
        auto *newNode = new TreeNode(newData);
        target = newNode;
    }
    TreeNode *position = target;
    while(true){
        if(position->data.key == newData.key){
            position->data = newData;
            position->isDeleted = false;
            return;
        }
        else if(position->data.key > newData.key){
            if(position->leftChild == nullptr){
                position->leftChild = new TreeNode(newData);
                return;
            }
            position = position->leftChild;
        }
        else if(position->data.key < newData.key){
            if(position->rightChild == nullptr){
                position->rightChild = new TreeNode(newData);
                return;
            }
            position = position->rightChild;
        }
    }
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::deleteData(const KeyType &key) {
    if(isEmpty()){
        return;
    }
    TreeNode *position = root;
    while(true){
        if(position->data.key == key){
            position->isDeleted = true;
            return;
        }
        else if(position->data.key > key){
            if(position->leftChild == nullptr){
                return;
            }
            position = position->leftChild;
        }
        else if(position->data.key < key){
            if(position->rightChild == nullptr){
                return;
            }
            position = position->rightChild;
        }
    }
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::clear(BST::TreeNode *target) {
    if(target == nullptr){
        return;
    }
    clear(target->leftChild);
    clear(target->rightChild);
    target = nullptr;
    delete target;
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::clear() {
    clear(this->root);
}

template<class KeyType, class ValueType>
int BST<KeyType, ValueType>::searchKey(const KeyType &key) {
    if(isEmpty()){
        return -1;
    }
    TreeNode *position = root;
    int rank_current = 1;
    while(true){
        if(position->data.key == key){
            if(position->isDeleted){return -1;}
            return rank_current + getSize(position->leftChild);
        }
        else if(position->data.key > key){
            if(position->leftChild == nullptr){
                return -1;
            }
            position = position->leftChild;
        }
        else if(position->data.key < key){
            if(position->rightChild == nullptr){
                return -1;
            }
            rank_current += getSize(position->leftChild);
            if(!position->isDeleted){
                rank_current++;
            }
            position = position->rightChild;
        }
    }
}

template<class KeyType, class ValueType>
key_value_pair<KeyType, ValueType> BST<KeyType, ValueType>::searchRank(TreeNode *&target,int64_t rank) {
    if(rank <= 0||rank > getSize(target)){
        return key_value_pair<KeyType,ValueType>();
    }
    int division = getSize(target->leftChild)+1;

    if(rank == division){
        return target->data;
    }

    else if(rank < division){
        return searchRank(target->leftChild,rank);
    }
    else if(rank > division){
        return searchRank(target->rightChild,rank - division);
    }
}

template<class KeyType, class ValueType>
key_value_pair<KeyType, ValueType> BST<KeyType, ValueType>::findPreviousItem(const KeyType &key) {
    int rank = this->searchKey(key);
    if(rank == -1){
        return key_value_pair<KeyType,ValueType>();
    }
    return searchRank(rank-1);
}

template<class KeyType, class ValueType>
key_value_pair<KeyType, ValueType> BST<KeyType, ValueType>::findFollowingItem(const KeyType &key) {
    int rank = this->searchKey(key);
    if(rank == -1){
        return key_value_pair<KeyType,ValueType>();
    }
    return searchRank(rank+1);
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::buildTree(BST::TreeNode *&subRoot,
                                        const std::vector<key_value_pair<KeyType, ValueType>> &dataSet) {
    if(dataSet.empty()){
        return;
    }
    for(auto item:dataSet){
        insertData(subRoot,item);
    }
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::writeDataToVector(BST::TreeNode *&target,
                                                std::vector<key_value_pair<KeyType, ValueType>> &dataSet) {
    if(target == nullptr){
        return;
    }
    writeDataToVector(target->leftChild,dataSet);
    if(!target->isDeleted){
        key_value_pair<KeyType,ValueType> tmp(target->data.key,target->data.value);
        dataSet.push_back(tmp);
    }
    writeDataToVector(target->rightChild,dataSet);
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::traverse() {
    std::vector<key_value_pair<KeyType,ValueType>> dataSet;
    writeDataToVector(root,dataSet);
    for(auto item:dataSet){
        std::cout << "(" << item.key << ',' <<item.value <<") ";
    }
}

template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::buildFromFile(const std::string &filepath) {
    std::vector<key_value_pair<KeyType, ValueType>> Set;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        KeyType key;
        ValueType value;
        char comma;
        if (iss >> key >> comma >> value && comma == ',') {
            Set.emplace_back(key, value);
        }
    }
    file.close();
    buildTree(root,Set);
}



#endif //BINARY_SEARCH_TREE_BST_H

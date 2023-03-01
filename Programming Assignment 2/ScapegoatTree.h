#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.

    /*Additional*/
    void printUpperBound() const {
        std::cout << upperBound << std::endl;
    }
    /*Additional*/

    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    
    Node<T> *deepCopy(const Node<T> *node);  
    
    int getHeight(const Node<T> *node) const;

    int getSize(const Node<T> *node) const;

    bool insertNode(Node<T> * &node, const T &element, bool & heightCondition);

    bool removeNode(Node<T> * &node, const T &element);
    
    void removeTree(Node<T> * &node);

    const Node<T> *get(const Node<T> *node, const T &element) const;

    void balanceTree(Node<T> * &node);

    void createArray(Node<T> * &node, T * array, int &index);

    Node<T> * createBST(T * array, int l, int r);

    void getCeiling(Node<T> *node, const T &element, T * &ans) const;

    void getFloor(Node<T> *node, const T &element, T * &ans) const;
    
    const T &getMin(const Node<T> *node) const;

    const T &getMax(const Node<T> *node) const;

    void getNext(Node<T> *node, const T &element, T * &ans) const;

    bool checkHeightCondition() const;

    bool checkUpperBoundCondition() const;

private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;
};

#endif //TREE_H

template<class T>
ScapegoatTree<T>::ScapegoatTree() {
    root = NULL;
    upperBound = 0;
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj) {
    root = deepCopy(obj.root);
    upperBound = obj.upperBound;
}

template<class T>
ScapegoatTree<T>::~ScapegoatTree() {
    removeAllNodes();
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const {
    return root == NULL;
}

template<class T>
int ScapegoatTree<T>::getHeight() const {
    return getHeight(root);
}

template<class T>
int ScapegoatTree<T>::getSize() const {
    return getSize(root);
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element) {
    bool heightCondition = true;
    return insertNode(root, element, heightCondition);
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element) {
    return removeNode(root, element);
}

template<class T>
void ScapegoatTree<T>::removeAllNodes() {
    removeTree(root);
    upperBound = 0;
}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const {
    const Node<T> *getNode = get(root, element);
    if (getNode == NULL)
        throw NoSuchItemException();
    return getNode -> element;
}

template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const {

    if (tp == preorder) {
        if (isEmpty()) {
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;

    } else if (tp == inorder) {
        // check if the tree is empty?
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }
        // the tree is not empty.
        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder) {
        if (isEmpty()) {
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const {

    if (tp == preorder) {
        if (node == NULL)
            return ;
        std::cout << "\t" << node->element;
        if (node -> left)
            std::cout << "," << std::endl;
        print(node -> left, preorder);

        if (node -> right)
            std::cout << "," << std::endl;
        print(node -> right, preorder);

    } else if (tp == inorder) {
        // check if the node is NULL?
        if (node == NULL)
            return;

        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }
        // then, output the node.
        std::cout << "\t" << node->element;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } else if (tp == postorder) {
        if (node == NULL)
            return ;

        print(node -> left, postorder);
        if (node -> left)
            std::cout << "," << std::endl;

        print(node -> right, postorder);
        if (node -> right)
            std::cout << "," << std::endl;

        std::cout << "\t" << node -> element;
    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const {

    // check if the tree is empty?
    if (isEmpty()) {
        // the tree is empty.
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }

    // the tree is not empty.

    // recursively output the tree.
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;

}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const {

    // check if the node is NULL?
    if (node == NULL)
        return;

    // output the indentation and the node.
    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    // first, output left subtree with one more indentation level.
    printPretty(node->left, indentLevel + 1, true);

    // then, output right subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false);
}

template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs) {
    if (this != &rhs){
        removeAllNodes();
        root = deepCopy(rhs.root);
        upperBound = rhs.upperBound;
    }
    return *this;
}

template<class T>
void ScapegoatTree<T>::balance() {
    balanceTree(root);
}

template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const {
    T * ans = NULL;
    getCeiling(root, element, ans);  
    if (!ans)
        throw NoSuchItemException();
    return *ans;
}

template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const {
    T * ans = NULL;
    getFloor(root, element, ans);
    if (!ans)
        throw NoSuchItemException();
    return *ans;
}

template<class T>
const T &ScapegoatTree<T>::getMin() const {
    if (isEmpty())
        throw NoSuchItemException();
    return getMin(root);
}

template<class T>
const T &ScapegoatTree<T>::getMax() const {
    if (isEmpty())
        throw NoSuchItemException();
    return getMax(root);
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const {
    T * ans = NULL;
    getNext(root, element, ans);
    if (!ans)
        throw NoSuchItemException();
    return *ans;
}

//My private functions


template<class T>
Node<T> *ScapegoatTree<T>::deepCopy(const Node<T> *node){
    if (node == NULL)
        return NULL;
    Node<T> *newNode = new Node<T>;
    newNode -> element = node -> element;
    newNode -> left = deepCopy(node -> left);
    newNode -> right = deepCopy(node -> right);
    return newNode;
}

template<class T>
int ScapegoatTree<T>::getHeight(const Node<T> *node) const {
    if (node == NULL)
        return -1;
    int leftHeight = getHeight(node -> left);
    int rightHeight = getHeight(node -> right);
    if (leftHeight > rightHeight)
        return 1 + leftHeight;
    else
        return 1 + rightHeight;
}

template<class T>
int ScapegoatTree<T>::getSize(const Node<T> *node) const {
    if (node == NULL)
        return 0;
    return 1 + getSize(node -> left) + getSize(node -> right);
}

template<class T>
bool ScapegoatTree<T>::insertNode(Node<T> * &node, const T &element, bool & heightCondition){
    if (node == NULL){
        node = new Node<T> (element, NULL, NULL);
        upperBound++;
        heightCondition = checkHeightCondition();
        return true;
    }
    else if (element < node -> element){
        if (insertNode(node -> left, element, heightCondition)){
            int leftChildSize = getSize(node -> left);
            int parentSize = 1 + leftChildSize + getSize(node -> right);
            if (!heightCondition && 3 * leftChildSize > 2 * parentSize){
                balanceTree(node);
                heightCondition = true;
            }
            return true;
        }
    }
    else if (element > node -> element)
        if (insertNode(node -> right, element, heightCondition)){
            int rightChildSize = getSize(node -> right);
            int parentSize = 1 + getSize(node -> left) + rightChildSize;
            if (!heightCondition && 3 * rightChildSize > 2 * parentSize){
                balanceTree(node);
                heightCondition = true;
            }
            return true;
        }
    return false;
}

template<class T>
bool ScapegoatTree<T>::removeNode(Node<T> * &node, const T &element){
    if (node == NULL)
        return false;
    if (element == node -> element){
        if (node -> left != NULL && node -> right != NULL){
            node -> element = getMax(node -> left);           
            return removeNode(node -> left, node -> element);
        }
        else {
            Node<T> * oldNode = node;
            node = (node -> left != NULL) ? node -> left : node -> right;
            
            delete oldNode;
            oldNode = NULL;
            if (!checkUpperBoundCondition()){
                balance();
                upperBound = getSize();
            }
            return true;
        }
    }
    else if (element < node -> element)
        return removeNode(node -> left, element);
    else
        return removeNode(node -> right, element);
}

template<class T>
void ScapegoatTree<T>::removeTree(Node<T> * &node){
    if (node == NULL)
        return ;
    removeTree(node -> left);
    removeTree(node -> right);
    delete node;
    node = NULL;
}

template<class T>
const Node<T> *ScapegoatTree<T>::get(const Node<T> *node, const T &element) const {
    if (node == NULL)
        return NULL;   
    if (element == node -> element)
        return node;
    else if (element < node -> element)
        return get(node -> left, element);
    else
        return get(node -> right, element);    
}

template<class T>
void ScapegoatTree<T>::balanceTree(Node<T> * &node){
    int size = getSize(node);
    T * array = new T[size];
    int index = 0;
    createArray(node, array, index);
    int l = 0, r = size - 1;
    node = createBST(array, l, r);
    delete[] array;
}

template<class T>
void ScapegoatTree<T>::createArray(Node<T> * &node, T * array, int &index){
    if (node == NULL)
        return ;
    createArray(node -> left, array, index);
    array[index++] = node -> element;
    createArray(node -> right, array, index);
    delete node;
    node = NULL;
}

template<class T>
Node<T> * ScapegoatTree<T>::createBST(T * array, int l, int r){
    if (l > r)
        return NULL;
    int m = l + (r - l) / 2;
    Node<T> * newNode = new Node<T>;
    newNode -> element = array[m];
    newNode -> left = createBST(array, l, m - 1);
    newNode -> right = createBST(array, m + 1, r);
    return newNode;
}

template<class T>
void ScapegoatTree<T>::getCeiling(Node<T> *node, const T &element, T * &ans) const {
    if (node == NULL)
        return ;
    if (element == node -> element){
        ans = &node -> element;
        return ;
    }
    else if (element < node -> element){
        ans = &node -> element;
        getCeiling(node -> left, element, ans);
    }
    else
        getCeiling(node -> right, element, ans);
}

template<class T>
void ScapegoatTree<T>::getFloor(Node<T> *node, const T &element, T * &ans) const {
    if (node == NULL)
        return ;
    if (element == node -> element){
        ans = &node -> element;
        return ;
    }
    else if (element > node -> element){
        ans = &node -> element;
        getFloor(node -> right, element, ans);
    }
    else
        getFloor(node -> left, element, ans);
}

template<class T>
const T &ScapegoatTree<T>::getMin(const Node<T> *node) const {
    if (node -> left == NULL)
        return node -> element;
    return getMin(node -> left);
}

template<class T>
const T &ScapegoatTree<T>::getMax(const Node<T> *node) const {
    if (node -> right == NULL)
        return node -> element;
    return getMax(node -> right);
}

template<class T>
void ScapegoatTree<T>::getNext(Node<T> *node, const T &element, T * &ans) const {
    if (node == NULL)
        return ;
    if (element < node -> element){
        ans = &node -> element;
        getNext(node -> left, element, ans);
    }
    else
        getNext(node -> right, element, ans);
}

template<class T>
bool ScapegoatTree<T>::checkHeightCondition() const {
    int height = getHeight();
    float logOfUpperBound = log(upperBound)/log(1.5);
    return height <= logOfUpperBound;
}

template<class T>
bool ScapegoatTree<T>::checkUpperBoundCondition() const {
    int size = getSize();
    return (float)upperBound/2 <= size && size <= upperBound;
}

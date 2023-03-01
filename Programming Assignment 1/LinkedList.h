#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};

template<class T>
LinkedList<T>::LinkedList()
{
    this -> head = NULL;
    this -> size  = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    this -> head = NULL;
    this -> size = 0;
    if (!obj.isEmpty()){         
        Node<T> * objNode = obj.getFirstNode();
        do {
            this -> append(objNode -> data);
            objNode = objNode -> next;
        } while (objNode != obj.getFirstNode());
    }
}

template<class T>
LinkedList<T>::~LinkedList()
{
    this -> removeAllNodes();
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if (this == &rhs)
        return *this;
    this -> removeAllNodes();
    if (!rhs.isEmpty()){         
        Node<T> * rhsNode = rhs.getFirstNode();
        do {
            this -> append(rhsNode -> data);
            rhsNode = rhsNode -> next;
        } while (rhsNode != rhs.getFirstNode());
    }
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    return size == 0;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    Node<T> * curNode = this -> getFirstNode();
    if (!curNode)
        return false;
    do {
        if (curNode == node)
            return true;
        curNode = curNode -> next;
    } while (curNode != this -> getFirstNode());
    return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    return this -> head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    Node<T> *lastNode = NULL;
    if (this -> head)
        lastNode = this -> head -> prev;
    return lastNode;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{    
    if (this -> isEmpty())
        return NULL;
    Node<T> *curNode = this -> getFirstNode();
    do {
        if (curNode -> data == data)
            return curNode;
        curNode = curNode -> next;
    } while (curNode != this -> getFirstNode());
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    if (index < 0 || index >= this -> size)
        return NULL;
    Node<T> *curNode = this -> head;
    for (int i = 0; i < index; i++)
        curNode = curNode -> next;
    return curNode;
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    Node<T> *newNode = new Node<T>(data);
    newNode -> next = newNode;
    newNode -> prev = newNode;
    Node<T> *lastNode = this -> getLastNode();
    if (lastNode){
        newNode -> next = lastNode -> next;
        newNode -> prev = lastNode;
        newNode -> next -> prev = newNode;
        newNode -> prev -> next = newNode;
    }
    else 
        this -> head = newNode;

    this -> size++;
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    Node<T> *newNode = new Node<T>(data);
    newNode -> next = newNode;
    newNode -> prev = newNode;
    Node<T> *firstNode = this -> getFirstNode();
    if (firstNode){
        newNode -> next = firstNode;
        newNode -> prev = firstNode -> prev;
        firstNode -> prev -> next = newNode;
        firstNode -> prev = newNode;
    }
    
    this -> head = newNode;
    this -> size++;
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    if (this -> containsNode(node)){
        Node<T> *newNode = new Node<T>(data);
        newNode -> next = node -> next;
        newNode -> prev = node;
        newNode -> next -> prev = newNode;
        newNode -> prev -> next = newNode;
        this -> size++;
    }
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    if (index >= 0 && index <= this -> size){       
        if (index == 0)
            this -> prepend(data);
        else {
            Node<T> *toInsertNode = this -> getNodeAtIndex(index-1);
            this -> insertAfterNode(data, toInsertNode);
        }
    }
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    Node<T> *curNode = this -> getFirstNode();
    if (!curNode)
        return -1;
    int index = 0;
    do {
        if (curNode == node)
            return index;
        curNode = curNode -> next;
        index++;
    } while (curNode != this -> getFirstNode());
    return -1;
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    if (newIndex >= this -> getSize())
        newIndex = this -> getSize() - 1;
    
    if (0 <= currentIndex && currentIndex < this -> getSize() && newIndex >= 0 && currentIndex != newIndex){                  
        Node<T> *curNode = this -> getNodeAtIndex(currentIndex);
        Node<T> *newNode;

        if (newIndex == 0){
            curNode -> prev -> next = curNode -> next;
            curNode -> next -> prev = curNode -> prev;
            newNode = this -> getFirstNode();
            curNode -> next = newNode;
            curNode -> prev = newNode -> prev;
            curNode -> next -> prev = curNode;
            curNode -> prev -> next = curNode;
            this -> head = curNode;
        }
        else {
            if (newIndex < currentIndex)
                newNode = this -> getNodeAtIndex(newIndex-1);
            else
                newNode = this -> getNodeAtIndex(newIndex);
            
            curNode -> prev -> next = curNode -> next;
            curNode -> next -> prev = curNode -> prev;
            if (currentIndex == 0)
                this -> head = curNode -> next;
            
            curNode -> next = newNode -> next;
            curNode -> prev = newNode;
            curNode -> next -> prev = curNode;
            curNode -> prev -> next = curNode;
        } 
    }    
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    if (this -> containsNode(node)){
        if (this -> getSize() == 1){
            delete node;
            this -> head = NULL;
        }
        else {
            node -> prev -> next = node -> next;
            node -> next -> prev = node -> prev;
            if (node == this -> getFirstNode())
                this -> head = node -> next;
            delete node;
        }
        this -> size--;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    Node<T> *curNode = this -> getFirstNode();
    while (!this -> isEmpty() && curNode -> data == data){
        Node<T> *nextNode = curNode -> next;
        this -> removeNode(curNode);
        curNode = nextNode;
    }
    if (!this -> isEmpty()){
        do {
            Node<T> *nextNode = curNode -> next;
            if (curNode -> data == data)
                this -> removeNode(curNode);
            curNode = nextNode;
        } while (curNode != this -> getFirstNode());
    }
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    if (index >= 0 && index < this -> getSize()){
        Node<T> *curNode = this -> getFirstNode();
        for (int i = 0; i < index; i++)
            curNode = curNode -> next;
        this -> removeNode(curNode);
    }
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    while (!this -> isEmpty())
        this -> removeNode(this -> getFirstNode());
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do {
        std::cout << *node << std::endl;
        node = node->next;
    }
    while (node != this->getFirstNode());
}

#endif //LINKEDLIST_H

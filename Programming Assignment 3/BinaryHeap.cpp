#include "BinaryHeap.h"
#include <iostream>
using namespace std;

BinaryHeap::BinaryHeap()
{
    // leave
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    for (int i = 0; i < HeapSize(); i++)
        if (elements[i].uniqueId == uniqueId)
            return false;
    
    HeapElement hel = {uniqueId, weight};
    elements.push_back(hel);
    perlocateUp(HeapSize()-1);
    return true;
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    if (HeapSize() == 0)
        return false;
    outUniqueId = elements.front().uniqueId;
    outWeight = elements.front().weight;
    if (HeapSize() == 1){
        elements.pop_back();
        return true;
    }
    else {
        elements[0] = elements.back();
        elements.pop_back();
        perlocateDown(0);
        return true;
    }
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    int index = 0;
    for (; index < HeapSize(); index++)
        if (elements[index].uniqueId == uniqueId)
            break;
    if (index == HeapSize())
        return false;
    
    double oldWeight = elements[index].weight;
    elements[index].weight = newWeight;
    if (newWeight < oldWeight) 
        perlocateUp(index);
    else
        perlocateDown(index);
    return true;
}

int BinaryHeap::HeapSize() const
{
    return elements.size();
}


void BinaryHeap::perlocateUp(int hole){
    HeapElement tmp = elements[hole];
    
    while (hole != 0 && tmp.weight < elements[(hole-1)/2].weight){
        elements[hole] = elements[(hole-1)/2];
        hole = (hole-1)/2;
    }
    elements[hole] = tmp;
}

void BinaryHeap::perlocateDown(int hole){
    
    int child = 0;
    HeapElement tmp = elements[hole];
    
    for (; hole*2+1 < HeapSize(); hole = child){
        child = hole*2+1;
        if (child != HeapSize()-1 && elements[child+1].weight < elements[child].weight)
            child++;

        if (elements[child].weight < tmp.weight)
            elements[hole] = elements[child];
        else
            break;
    }
    elements[hole] = tmp;
}

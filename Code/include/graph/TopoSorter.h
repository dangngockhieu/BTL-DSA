#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "hash/xMap.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
template <class T, class V>
using XHashMap = xMap<T, V>;

template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0){
        this->graph = graph;
        this->hash_code = hash_code;
    }   
    DLinkedList<T> sort(int mode=0, bool sorted=true){  
        if(mode == DFS) return dfsSort(sorted);
        else if(mode == BFS) return bfsSort(sorted);
        return DLinkedList<T>(); 
    }
    // BFS
    DLinkedList<T> bfsSort(bool sorted=true){
        DLinkedListSE<T> zeroList = listOfZeroInDegrees();
        if (sorted) {
            zeroList.sort();
        }
        xMap<T, int> inMap = vertex2inDegree(hash_code);
        DLinkedList<T> list;
        Queue<T> queue;
        for (auto i = zeroList.begin(); i != zeroList.end(); ++i) {
            queue.push(*i);
        }
        while (!queue.empty()) {
            T vertex = queue.pop();
            list.add(vertex);
            DLinkedList<T> e = graph->getOutwardEdges(vertex);
            for (auto i = e.begin(); i != e.end(); ++i) {
                T neighbor = (*i);
                inMap.put(neighbor, inMap.get(neighbor) - 1);
                if (inMap.get(neighbor) == 0) {
                    queue.push(neighbor);
                }
            }
        }

        return list;
    }
    // DFS
     DLinkedList<T> dfsSort(bool sorted=true){
        DLinkedListSE<T> zeroList = listOfZeroInDegrees();
        if (sorted) {
            zeroList.sort();
        }

        xMap<T, bool> visited(hash_code);
        DLinkedList<T> list;

        for (auto i = zeroList.begin(); i != zeroList.end(); ++i) {
            if (!visited.containsKey(*i)) {
                dfsVisit(*i, visited, list);
            }
        }

        return list;
    }
    void dfsVisit(T vertex, xMap<T, bool>& visited, DLinkedList<T>& list) {
        visited.put(vertex, true);
        DLinkedList<T> edges = graph->getOutwardEdges(vertex);
        for (auto i = edges.begin(); i != edges.end(); ++i) {
            T neighbor = (*i);
            if (!visited.containsKey(neighbor)) {
                dfsVisit(neighbor, visited, list);
            }
        }
        list.add(0,vertex);
    }


protected:

    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int)){
        xMap<T, int> inMap(hash);
        DLinkedList<T> v = graph->vertices(); 
    
        for (auto i = v.begin(); i != v.end(); ++i) {
            inMap.put((*i), 0);
        }
    
        for (auto i = v.begin(); i != v.end(); ++i) {
            DLinkedList<T> e = graph->getOutwardEdges(*i);
            for (auto eIdx = e.begin(); eIdx != e.end(); ++eIdx) {
                T neighbor = *eIdx;
                inMap.put(neighbor, inMap.get(neighbor) + 1);
            }
        }
    
        return inMap;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int)){
        xMap<T, int> outMap(hash);
        DLinkedList<T> v = graph->vertices(); 
    
        for (auto i = v.begin(); i != v.end(); ++i) {
            outMap.put((*i), 0);
        }
    
        for (auto i = v.begin(); i != v.end(); ++i) {
            DLinkedList<T> e = graph->getInwardEdges(*i);
            for (auto eIdx = e.begin(); eIdx != e.end(); ++eIdx) {
                T neighbor = *eIdx;
                outMap.put(neighbor, outMap.get(neighbor) + 1);
            }
        }
    
        return outMap;
    }
    DLinkedList<T> listOfZeroInDegrees(){
        DLinkedList<T> zeroList;
        xMap<T, int> inMap = vertex2inDegree(hash_code);
        DLinkedList<T> keys = inMap.keys();
        for (auto i = keys.begin(); i != keys.end(); ++i) {
            if (inMap.get(*i) == 0) {
                zeroList.add(*i);
            }
        }
        return zeroList;
    }

}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */


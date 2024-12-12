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
        DLinkedListSE<T> zero = listOfZeroInDegrees();
        if (sorted) {
            zero.sort();
        }
        xMap<T, int> inMap = vertex2inDegree(hash_code);
        DLinkedList<T> BFS_list;
        Queue<T> q;
        for (auto i = zero.begin(); i != zero.end(); ++i) {
            q.push(*i);
        }
        while (!q.empty()) {
            T v = q.pop();
            BFS_list.add(v);
            DLinkedList<T> e = graph->getOutwardEdges(v);
            auto i = e.begin();
            while ( i != e.end()) {
                T neighbor = (*i);
                inMap.put(neighbor, inMap.get(neighbor) - 1);
                if (inMap.get(neighbor) == 0) {
                    q.push(neighbor);
                }
                ++i;
            }
        }

        return BFS_list;
    }
    // DFS
     DLinkedList<T> dfsSort(bool sorted=true){
        DLinkedListSE<T> zero = listOfZeroInDegrees();
        if (sorted) {
            zero.sort();
        }
        xMap<T, bool> visited(hash_code);
        DLinkedList<T> DFS_list;
        auto i = zero.begin();
        while ( i != zero.end()) {
            if (visited.containsKey(*i)==false) {
                help(*i, visited, DFS_list);
            }
            ++i;
        }

        return DFS_list;
    }
    void help(T vertex, xMap<T, bool>& visited, DLinkedList<T>& list) {
        visited.put(vertex, true);
        DLinkedList<T> edges = graph->getOutwardEdges(vertex);
        auto i = edges.begin();
        while ( i != edges.end()) {
            T neighbor = (*i);
            if (!visited.containsKey(neighbor)) {
                help(neighbor, visited, list);
            }
            ++i;
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
            auto eIdx = e.begin();
            while (eIdx != e.end()) {
                T neighbor = *eIdx;
                inMap.put(neighbor, inMap.get(neighbor) + 1);
                ++eIdx;
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
        auto i = v.begin();
        while (i != v.end()) {
            DLinkedList<T> e = graph->getInwardEdges(*i);
            for (auto eIdx = e.begin(); eIdx != e.end(); ++eIdx) {
                T neighbor = *eIdx;
                outMap.put(neighbor, outMap.get(neighbor) + 1);
            }
            ++i;
        }
        return outMap;
    }
    DLinkedList<T> listOfZeroInDegrees(){
        DLinkedList<T> zeroList;
        xMap<T, int> inMap = vertex2inDegree(hash_code);
        DLinkedList<T> vertices = graph->vertices();
        for (auto i = vertices.begin(); i != vertices.end(); ++i) {
            if (inMap.get(*i) == 0) {
                zeroList.add(0,*i);
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


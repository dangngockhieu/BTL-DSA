#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
//#include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    void connect(T from, T to, float weight=0){
        typename AbstractGraph<T>::VertexNode* end = this->getVertexNode(to);
        typename AbstractGraph<T>::VertexNode* begin = this->getVertexNode(from);
        if (begin == nullptr) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (end == nullptr) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
            begin->connect(end, weight);
    }
    void disconnect(T from, T to){
        typename AbstractGraph<T>::VertexNode* end = this->getVertexNode(to);
        typename AbstractGraph<T>::VertexNode* begin = this->getVertexNode(from);
        if (begin == nullptr) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (end == nullptr) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        typename AbstractGraph<T>::Edge* e = begin->getEdge(end);
        typename AbstractGraph<T>::Edge edge(begin,end);
        if(e != nullptr){
            begin->removeTo(end);
        }
        else {
            throw EdgeNotFoundException(this->edge2Str(edge));
        }
        
    }
    void remove(T vertex){
        typename AbstractGraph<T>::VertexNode* v = this->getVertexNode(vertex);
        if (v == nullptr) {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        for (auto i = this->nodeList.begin(); i != this->nodeList.end(); ++i) {
        typename AbstractGraph<T>::VertexNode* node = *i;
        if (node->getEdge(v) != nullptr) {
            node->removeTo(v);
        }
        if (v->getEdge(node) != nullptr) {
            v->removeTo(node);
        }
        }
        this->nodeList.removeItem(v);
        delete v;
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; ++i) {
            graph->add(vertices[i]);
        }
        for (int i = 0; i < nedges; ++i) {
            typename AbstractGraph<T>::VertexNode* from = graph->getVertexNode(edges[i].from);
            typename AbstractGraph<T>::VertexNode* to = graph->getVertexNode(edges[i].to);
            from->connect(to,edges[i].weight);
        }
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
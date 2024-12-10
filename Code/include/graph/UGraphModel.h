#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
//#include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0){
        typename AbstractGraph<T>::VertexNode* end = this->getVertexNode(to);
        typename AbstractGraph<T>::VertexNode* begin = this->getVertexNode(from);
        if (!begin) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (!end) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        if (begin->equals(end)) {
            begin->connect(end, weight);
        } else {
            begin->connect(end, weight);
            end->connect(begin, weight);
        }
    }
    void disconnect(T from, T to){
        typename AbstractGraph<T>::VertexNode* end = this->getVertexNode(to);
        typename AbstractGraph<T>::VertexNode* begin = this->getVertexNode(from);
        if (!begin) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (!end) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        typename AbstractGraph<T>::Edge* e = begin->getEdge(end);
        typename AbstractGraph<T>::Edge edge(begin,end);
        if (e == nullptr) {
            throw EdgeNotFoundException(this->edge2Str(edge));
        }
        begin->removeTo(end);
        if(begin->equals(end)==false){
            end->removeTo(begin);
        }
    }
    void remove(T vertex){
        typename AbstractGraph<T>::VertexNode* v = this->getVertexNode(vertex);
        if (v == nullptr) {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        for (auto i = this->nodeList.begin(); i != this->nodeList.end(); ++i) {
        typename AbstractGraph<T>::VertexNode* node = *i;
        if(node->equals(v)){
            if (node->getEdge(v) != nullptr)
            node->removeTo(v);
        }
        else{
            if (node->getEdge(v) != nullptr) {
                node->removeTo(v);
            }
            if (v->getEdge(node) != nullptr) {
                v->removeTo(node);
            }
        }
    }
    this->nodeList.removeItem(v);
    delete v;
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T>* graph = new UGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; ++i) {
            graph->add(vertices[i]);
        }
        for (int i = 0; i < nedges; ++i) {
            typename AbstractGraph<T>::VertexNode* from = graph->getVertexNode(edges[i].from);
            typename AbstractGraph<T>::VertexNode* to = graph->getVertexNode(edges[i].to);
            from->connect(to,edges[i].weight);
            if(!graph->vertexEQ(edges[i].from,edges[i].to)){
                to->connect(from,edges[i].weight);
            }
        }
        return graph;
    }
};

#endif /* UGRAPHMODEL_H */





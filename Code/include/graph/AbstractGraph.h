#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

template<class T>
class AbstractGraph: public IGraph<T>{
public:
    class Edge; //forward declaration
    class VertexNode; //forward declaration
    class Iterator; //forward declaration
    
private:
protected:
    //Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode*> nodeList; 
    
    //Function pointers:
    bool (*vertexEQ)(T&, T&); //to compare two vertices
    string (*vertex2str)(T&); //to obtain string representation of vertices
    
    
    VertexNode* getVertexNode(T& vertex){
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        while(it != nodeList.end()){
            VertexNode* node = *it;
            if(vertexEQ(node->vertex, vertex) ) return node;
            it++;
        }
        return 0;
    }
    string vertex2Str(VertexNode& node){
        return vertex2str(node.vertex);
    }
    string edge2Str(Edge& edge){
        stringstream os;
        os << "E("
                << vertex2str(edge.from->vertex)
                << ","
                << vertex2str(edge.to->vertex)
                << ")";
        return os.str();
    }
    
public:
    AbstractGraph(
            bool (*vertexEQ)(T&, T&)=0, 
            string (*vertex2str)(T&)=0){
        
        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph(){}
    
    typedef bool (*vertexEQFunc)(T&, T&);
    typedef string (*vertex2strFunc)(T&);
    vertexEQFunc getVertexEQ(){
        return this->vertexEQ;
    }
    vertex2strFunc getVertex2Str(){
        return this->vertex2str;
    }

    
//////////////////////////////////////////////////////////////////////
//////////////////// IMPLEMENTATION of IGraph API ////////////////////
//////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight=0) =0;
    virtual void disconnect(T from, T to)=0;
    virtual void remove(T vertex)=0;
    
    
    
    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex) {
        if(this->contains(vertex)==false){
            VertexNode* node= new VertexNode(vertex,vertexEQ,vertex2str);
            this->nodeList.add(node);
        }
    }
    virtual bool contains(T vertex){
       if(this->getVertexNode(vertex) == nullptr){
            return false;
        }
        return true;
    }
    virtual float weight(T from, T to){
        VertexNode* end = this->getVertexNode(to);
        VertexNode* begin = this->getVertexNode(from);
        if (begin == nullptr) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (end == nullptr) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        typename AbstractGraph<T>::Edge* e = begin->getEdge(end);
        typename AbstractGraph<T>::Edge edge(begin,end);
        if (e == nullptr) {
            throw EdgeNotFoundException(this->edge2Str(edge));
        }
        return e->weight;
    }
    virtual DLinkedList<T> getOutwardEdges(T from){
        VertexNode* from_node = this->getVertexNode(from);
        if (from_node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        return from_node->getOutwardEdges();
    }
    
    virtual DLinkedList<T>  getInwardEdges(T to){
       VertexNode* to_node = this->getVertexNode(to);
        if (to_node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        DLinkedList<T> in;
        for(auto i=this->nodeList.begin();i!=this->nodeList.end();++i){
            for(auto j=((*i)->adList).begin();j!=((*i)->adList).end();++j){
                if((*j)->to->equals(to_node)){
                    in.add((*j)->from->getVertex());
                }
            }
        }
        return in;
    }
    
    virtual int size() {
        //TODO
        return nodeList.size();
    }
    virtual bool empty(){
        //TODO
        return nodeList.empty();
    };
    virtual void clear(){
        for(auto i=this->nodeList.begin();i!=this->nodeList.end();++i){
            ((*i)->adList).clear();
        }
        for(auto i=this->nodeList.begin();i!=this->nodeList.end();++i){
            delete *i;
        }
        this->nodeList.clear();
    }
    virtual int inDegree(T vertex){
        VertexNode* node = this->getVertexNode(vertex);
        if (!node) {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        return node->inDegree();
    }
    virtual int outDegree(T vertex){
        VertexNode* node = this->getVertexNode(vertex);
        if (node == nullptr) {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        return node->outDegree();
    }
    
    virtual DLinkedList<T> vertices(){
        DLinkedList<T> v_list;
        for(auto i = nodeList.begin();i != nodeList.end(); ++i){
            v_list.add((*i)->getVertex());
        }
        return v_list;
    }
    virtual bool connected(T from, T to){
        VertexNode* end = this->getVertexNode(to);
        VertexNode* begin = this->getVertexNode(from);
        if (!begin) {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (!end) {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        if (begin->getEdge(end) != nullptr){
            return true;
        }
        return false;
    }
    void println(){
        cout << this->toString() << endl;
    }
    virtual string toString(){
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;
        
        os << left << setw(12) << "Edges:" << endl;
        
        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()){
                Edge* edge = *edgeIt;
                os << edge->toString() << endl;
                
                edgeIt++;
            }
            
            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }
    
    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it; 
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
//BEGIN of VertexNode    
    class VertexNode{
    private:
        template<class U>
        friend class UGraphModel; //UPDATED: added
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge*> adList; 
        friend class Edge;
        friend class AbstractGraph;
        
        bool (*vertexEQ)(T&, T&);
        string (*vertex2str)(T&);
        
    public:
        VertexNode():adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){}
        VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&))
            :adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }
        T& getVertex(){
            return vertex;
        }
        void connect(VertexNode* to, float weight=0){
            if(this->getEdge(to)==nullptr){
                Edge* e = new Edge(this, to, weight);
                this->adList.add(e);
                this->outDegree_+=1;
                to->inDegree_+=1;
            }
            else{
                this->getEdge(to)->weight=weight;
            }
        }
        DLinkedList<T> getOutwardEdges(){
            DLinkedList<T> out;
            for (auto i = adList.begin();i != adList.end();++i){
                out.add((*i)->to->getVertex());
            }
            return out;
        }

        Edge* getEdge(VertexNode* to){
            auto i = adList.begin();
            while(i != adList.end()){
                if((*i)->to->equals(to)){
                    return *i;
                }
                ++i;
            }
            return nullptr;
        }
        bool equals(VertexNode* node){
            return this->vertexEQ(vertex,node->vertex);
        }
        
        void removeTo(VertexNode* to){
            Edge* e = this->getEdge(to);
            Edge* edge= e;
            if (e != nullptr) {
                Edge* edge = e;
                this->adList.removeItem(e);
                delete edge;
                to->inDegree_--;
                this->outDegree_--;
            }
        }
        int inDegree(){
            return this->inDegree_;
        }
        int outDegree(){
            return this->outDegree_;
        }
        string toString(){
            stringstream os;
            os << "V("
                    << this->vertex << ", "
                    << "in: " << this->inDegree_ << ", "
                    << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
//END of VertexNode    
    
//BEGIN of Edge
    class Edge{
    private:
        VertexNode* from;
        VertexNode* to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;
        
    public:
        Edge(){}
        Edge(VertexNode* from, VertexNode* to, float weight=0){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        
        bool equals(Edge* edge){
            bool x1 = this->from->equals(edge->from);
            bool x2 = this->to->equals(edge->to);
            return x1 && x2;
        }

        static bool edgeEQ(Edge*& edge1, Edge*& edge2){
            return edge1->equals(edge2);
        }
        string toString(){
            stringstream os;
            os << "E("
                    << this->from->vertex
                    << ","
                    << this->to->vertex
                    << ","
                    << this->weight
                    << ")";
            return os.str();
        }
    };
 //END of Edge
    

//BEGIN of Iterator
public:
    class Iterator{
    private:
        typename DLinkedList<VertexNode*>::Iterator nodeIt;
        
    public:
        Iterator(AbstractGraph<T>* pGraph=0, bool begin=true){
            if(begin) {
                if(pGraph != 0) nodeIt = pGraph->nodeList.begin();
            }
            else{
                if(pGraph != 0) nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator& operator=(const Iterator& iterator){
            this->nodeIt = iterator.nodeIt;
            return *this;
        }
        
        T& operator*(){
            return (*nodeIt)->vertex;
        }
        
        bool operator!=(const Iterator& iterator){
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            nodeIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
//END of Iterator
};

#endif

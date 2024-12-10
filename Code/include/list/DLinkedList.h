#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H
#include "list/IList.h"
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0){
        cout << toString(item2str) << endl;
    }
    
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0){
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size){
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++){
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }
    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list){
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end()){
            delete *it;
            it++;
        }
    }
    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin(){
       return Iterator(this, true);
    }
    
    Iterator end(){
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin(){
        return BWDIterator(this, true);
    }
    
    BWDIterator bend(){
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)){
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node{
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0){
            this->next = next;
            this->prev = prev;
        }
        
        Node(T data, Node *next = 0, Node *prev = 0){
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator{
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true){
            if (begin){
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else{
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator){
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        
        void remove(void (*removeItemData)(T) = 0){
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*(){
            return pNode->data;
        }
        
        bool operator!=(const Iterator &iterator){
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++(){
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int){
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    class BWDIterator {
private:
    DLinkedList<T> *pList;
    Node *pNode;

public:
    BWDIterator(DLinkedList<T> *pList = 0, bool begin = true) {
        this->pList=pList;
        if (begin==1) {
            if(pList==nullptr) pNode=nullptr;
            else pNode=pList->tail->prev;
        } else {
            if(pList==nullptr) pNode=nullptr;
            else pNode=pList->head;
        }
    }

    bool operator!=(const BWDIterator &iterator) {
        return pNode != iterator.pNode;
    }

    T &operator*() {
        return pNode->data;
    }
void remove(void (*removeItemData)(T) = 0){
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->next; 
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }
    // Prefix ++
    BWDIterator &operator--() {
        if (pNode!=nullptr) pNode = pNode->prev;
        return *this;
    }

    // Postfix++
    BWDIterator operator--(int) {
        BWDIterator temp = *this;
        --*this;
        return temp;
    }
};
};
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;
////////METHOD DEFNITION////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    this->head = new Node();
    this->tail = new Node();
    head->next = tail;  
    tail->prev = head;
    this->count = 0;
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list){
    head=new Node();
    tail=new Node();
    this->count = list.count;
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    
    Node* curlist=list.head->next;
    Node* cur=this->head;
    while(curlist!=list.tail){
        Node *temp=new Node(curlist->data);
        cur->next=temp;
        temp->prev=cur;
        cur=cur->next;
        curlist=curlist->next;
    }
    cur->next=tail;
    tail->prev=cur; 
    
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list){
     head=new Node();
    tail=new Node();
    this->count = list.count;
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    
 if (this != &list) {
        clear();
    Node* cur = list.tail->prev;
    int i=list.count-1;
    while(i>=0){
        T d=cur->data;
        this->add(0,d);
        cur=cur->prev;
        i--;
    }
    }
    return *this;
   
}

template <class T>
DLinkedList<T>::~DLinkedList(){
     if (deleteUserData != nullptr) deleteUserData(this);
    Node* cur=tail->prev;
    Node* temp;
    while(cur!=head){
        temp=cur;
        cur=cur->prev;
        delete temp;
        this->count-=1;
    }
    if(this->count!=0) this->count=0;
    delete head;
    delete tail;
}

template <class T>
void DLinkedList<T>::add(T e){
    Node* temp= new Node(e);
    if(this->size()==0){
        head->next=temp;
        temp->prev=head;
        temp->next=tail;
        tail->prev=temp;
    }
    else{
        tail->prev->next=temp;
        temp->prev=tail->prev;
        temp->next=tail;
        tail->prev=temp;
    }
    count++;
}

template <class T>
void DLinkedList<T>::add(int index, T e){
    if (index<0||index>this->size()) throw std::out_of_range("Index is out of range!");
    if(index==this->size()){
    	this->add(e);
	}
    else if(index==0){
        Node* temp = new Node(e);
        Node*first=head->next;
        head->next=temp;
        temp->prev=head;
        temp->next=first;
        first->prev=temp;
        count++;
    }
    else{
    Node* temp = new Node(e);
    int i=0;
    Node*cur=this->head->next;
    while(i<index){
        cur=cur->next;
        i++;
    }
    cur=cur->prev;
    temp->next=cur->next;
    cur->next->prev=temp;
    cur->next=temp;
    temp->prev=cur;
    count++;}    
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index){
    if (index < 0 || index > this->size()) throw std::out_of_range("Index is out of range!");
    if(index<=count/2){
    	Node*cur=head->next;
        int i=0;
        while(i<index){
            cur=cur->next;
            i++;
        }
        cur=cur->prev;
        return cur;
	}
    	Node*cur=tail;
        int i=this->size();
        while(i>index){
            cur=cur->prev;
            i--;
        }
        cur=cur->prev;
        return cur;
}

template <class T>
T DLinkedList<T>::removeAt(int index){
    if (index < 0 || index >= this->size()) throw std::out_of_range("Index is out of range!");
    Node*cur=head->next;
    for(int i=0;i<index;i++){
        cur=cur->next;
    } 
    T trave=cur->data;
    Node*pre=cur->prev;
    pre->next=cur->next;
    cur->next->prev=pre;
    delete cur;
    this->count--;
    return trave;
}

template <class T>
bool DLinkedList<T>::empty(){
    if(this->size()==0) return 1;
    return 0;
}

template <class T>
int DLinkedList<T>::size(){
    return this->count;
}

template <class T>
void DLinkedList<T>::clear(){
     if (deleteUserData != nullptr) deleteUserData(this);
    Node* cur=tail->prev;
    Node* temp;
    while(cur!=head){
        temp=cur;
        cur=cur->prev;
        delete temp;
        this->count-=1;
    }
    head->next = tail;
    tail->prev = head;
    if(this->empty()==0) this->count=0;
}

template <class T>
T &DLinkedList<T>::get(int index){
	if (index < 0 || index >= this->size()) throw std::out_of_range("Index is out of range!");
    Node*cur=tail->prev;
    int i=this->size()-1;
    while(i>index){
        cur=cur->prev;
        i--;
    }
    return cur->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item){
    Node*cur=tail->prev;
    int i=this->size()-1;
    int index=this->size();
    if(index==0) return -1;
    while(cur!=this->head){
        T compare=cur->data;
        if(equals(item,compare,itemEqual)==1) {if(i<index) index=i;}
        cur=cur->prev;
        i--;
    }
    if(index==this->size()) return -1;
    return index;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)){
    if(this->contains(item)==0) return false;
    int index=this->indexOf(item);
    Node* del=this->getPreviousNodeOf(index);
    Node* pre=del;
    del=del->next;
    pre->next=del->next;
    del->next->prev=pre;
    if(removeItemData!=nullptr) removeItemData(del->data);
    delete del;
    this->count=this->size()-1;
    return true;
}

template <class T>
bool DLinkedList<T>::contains(T item){
    if(this->indexOf(item)==-1) return 0;
    return 1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &)){
    stringstream final;
    final << "[";
    int i=0;
    while(i<this->size()){
        if(i<this->size()-1){
        if(item2str!=nullptr) {final<<item2str(this->get(i));final<<", ";}
        else {final<<this->get(i);final<<", ";}
        }
        else{
        if(item2str!=nullptr) {final<<item2str(this->get(i));}
        else {final<<this->get(i);} 
        }
        i++;
    }
    final << "]";
    return final.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list){
    clear();
    Node* cur = list.tail->prev;
    int i=list.count-1;
    while(i>=0){
        T d=cur->data;
        this->add(this->size(),d);
        cur=cur->prev;
        i--;
    }

}

template <class T>
void DLinkedList<T>::removeInternalData(){
    if (deleteUserData != nullptr) deleteUserData(this);
    Node* cur=tail->prev;
    Node* temp;
    while(cur!=head){
        temp=cur;
        cur=cur->prev;
        delete temp;
        this->count-=1;
    }
    head->next = tail;
    tail->prev = head;
    if(this->count!=0) this->count=0;
}

#endif /* DLINKEDLIST_H */

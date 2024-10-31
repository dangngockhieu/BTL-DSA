#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H
#include "list/IList.h"
#include <iostream>
#include <sstream>
#include <type_traits>
using namespace std;

template <class T> class DLinkedList : public IList<T> {
public:
  class Node;
  class Iterator;
  class BWDIterator;

protected:
  Node *head; // this node does not contain user's data
  Node *tail; // this node does not contain user's data
  int count;
  bool (*itemEqual)(T &lhs, T &rhs); // function pointer: test if two items
                                     // (type: T&) are equal or not
  void (*deleteUserData)(
      DLinkedList<T> *); // function pointer: be called to remove items (if they
                         // are pointer type)

public:
  DLinkedList(void (*deleteUserData)(DLinkedList<T> *) = 0,
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

  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }

  void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  bool contains(T array[], int size) {
    int idx = 0;
    for (DLinkedList<T>::Iterator it = begin(); it != end(); it++) {
      if (!equals(*it, array[idx++], this->itemEqual))
        return false;
    }
    return true;
  }
  /* free(DLinkedList<T> *list):
   *  + to remove user's data (type T, must be a pointer type, e.g.: int*,
   * Point*)
   *  + if users want a DLinkedList removing their data,
   *      he/she must pass "free" to constructor of DLinkedList
   *      Example:
   *      DLinkedList<T> list(&DLinkedList<T>::free);  */

  static void free(DLinkedList<T> *list) {
    typename DLinkedList<T>::Iterator it = list->begin();
    while (it != list->end()) {
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
   }  */

  Iterator begin() { return Iterator(this, true); }

  Iterator end() { return Iterator(this, false); }

  /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
   * Example: assume "list" is object of DLinkedList

   DLinkedList<char>::BWDIterator it;
   for(it = list.last(); it != list.beforeFirst(); it--){
          char item = *it;
          std::cout << item; //print the item
   }  */

  BWDIterator bbegin() { return BWDIterator(this, true); }

  BWDIterator bend() { return BWDIterator(this, false); }

protected:
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
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
  class Node {
  public:
    T data;
    Node *next;
    Node *prev;
    friend class DLinkedList<T>;

  public:
    Node(Node *next = 0, Node *prev = 0) {
      this->next = next;
      this->prev = prev;
    }

    Node(T data, Node *next = 0, Node *prev = 0) {
      this->data = data;
      this->next = next;
      this->prev = prev;
    }
  };

  class Iterator {
  private:
    DLinkedList<T> *pList;
    Node *pNode;

  public:
    Iterator(DLinkedList<T> *pList = 0, bool begin = true) {
      if (begin) {
        if (pList != 0)
          this->pNode = pList->head->next;
        else
          pNode = 0;
      } else {
        if (pList != 0)
          this->pNode = pList->tail;
        else
          pNode = 0;
      }
      this->pList = pList;
    }

    Iterator &operator=(const Iterator &iterator) {
      this->pNode = iterator.pNode;
      this->pList = iterator.pList;
      return *this;
    }

    void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
      if (removeItemData != 0)
        removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }

    T &operator*() { return pNode->data; }

    bool operator!=(const Iterator &iterator) {
      return pNode != iterator.pNode;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      pNode = pNode->next;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
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
      this->pList = pList;
      if (begin) {
        this->pNode = pList ? pList->tail->prev : nullptr;
      } else {
        this->pNode = pList ? pList->head : nullptr;
      }
    }

    bool operator!=(const BWDIterator &iterator) {
      return pNode != iterator.pNode;
    }

    T &operator*() { return pNode->data; }
    void remove(void (*removeItemData)(T) = 0) {
      pNode->prev->next = pNode->next;
      pNode->next->prev = pNode->prev;
      Node *pNext = pNode->next; // MUST prev, so iterator++ will go to end
      if (removeItemData != 0)
        removeItemData(pNode->data);
      delete pNode;
      pNode = pNext;
      pList->count -= 1;
    }
    // Prefix ++
    BWDIterator &operator++() {
      if (pNode)
        pNode = pNode->prev;
      return *this;
    }

    // Postfix ++
    BWDIterator operator++(int) {
      BWDIterator temp = *this;
      ++*this;
      return temp;
    }
  };
};
// Define a shorter name for DLinkedList:

template <class T> using List = DLinkedList<T>;
////////METHOD DEFNITION////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *),
                            bool (*itemEqual)(T &, T &)) {
  this->deleteUserData = deleteUserData;
  this->itemEqual = itemEqual;
  count = 0;                            
  tail = new Node();
  head = new Node();
  tail->prev = head;
  head->next = tail;
  
}

template <class T> DLinkedList<T>::DLinkedList(const DLinkedList<T> &list) {
  copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list) {
  if (this != &list) {
    removeInternalData();
    copyFrom(list);
  }
  return *this;
}

template <class T> DLinkedList<T>::~DLinkedList() {
  removeInternalData();
  delete head;
  delete tail;
}

template <class T> void DLinkedList<T>::add(T e) {
  Node *moi = new Node(e);
  Node *tmp = tail->prev;
  tmp->next = moi;
  moi->prev = tmp;
  moi->next = tail;
  tail->prev = moi;
  count++;
}

template <class T> void DLinkedList<T>::add(int index, T e) {
  if (index < 0 || index > count) {
    throw std::out_of_range("Index is out of range!");
  }
  if (index == count) {
    add(e);
    return;
  }
  Node *moi = new Node(e);
  if (index <= count / 2) {
    Node *tmp1 = head;
    for (int i = 0; i < index; i++) {
      tmp1 = tmp1->next;
    }
    Node *tmp2 = tmp1->next;
    tmp1->next = moi;
    moi->prev = tmp1;
    moi->next = tmp2;
    tmp2->prev = moi;
  } else {
    Node *tmp2 = tail;
    for (int i = count; i > index; i--) {
      tmp2 = tmp2->prev;
    }
    Node *tmp1 = tmp2->prev;
    tmp1->next = moi;
    moi->prev = tmp1;
    moi->next = tmp2;
    tmp2->prev = moi;
  }
  count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) {
  if (index <= count / 2) {
    Node *tmp1 = head;
    for (int i = 0; i < index; i++) {
      tmp1 = tmp1->next;
    }
    return tmp1;
  } else {
    Node *tmp1 = tail;
    for (int i = count; i > index; i--) {
      tmp1 = tmp1->prev;
    }
    return tmp1->prev;
  }
}

template <class T> T DLinkedList<T>::removeAt(int index) {
  if (index < 0 || index >= count) {
    throw std::out_of_range("Index is out of range!");
  }
  Node *tmp1 = getPreviousNodeOf(index);
  Node *tmp2 = tmp1->next;
  Node *tmp3 = tmp2->next;
  tmp1->next = tmp3;
  tmp3->prev = tmp1;
  T a = tmp2->data;
  delete tmp2;
  count--;
  return a;
}

template <class T> bool DLinkedList<T>::empty() { return count == 0; }

template <class T> int DLinkedList<T>::size() { return count; }

template <class T> void DLinkedList<T>::clear() {
  Node *current = head->next;
  while (current != tail) {
    Node *tmp = current;
    current = current->next;
    delete tmp;
  }
  head->next = tail;
  tail->prev = head;
  count = 0;
}

template <class T> T &DLinkedList<T>::get(int index) {
  if (index < 0 || index >= count) {
    throw std::out_of_range("Index is out of range!");
  }
  if ((index) <= count / 2) {
    Node *tmp = head;
    for (int i = 0; i <= index; i++) {
      tmp = tmp->next;
    }
    return tmp->data;
  } else {
    Node *tmp = tail;
    for (int i = count; i > index; i--) {
      tmp = tmp->prev;
    }
    return tmp->data;
  }
}

template <class T> int DLinkedList<T>::indexOf(T item) {
  Node *tmp = head;
  for (int i = 0; i < count; i++) {
    tmp = tmp->next;
    if (equals(tmp->data, item, itemEqual)) {
      return i;
    }
  }
  return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)) {
  Node *current = head->next;
  while (current != tail) {
    if (equals(current->data, item, itemEqual)) {
      Node *ab = current;
      ab->prev->next = ab->next;
      ab->next->prev = ab->prev;
      if (removeItemData) {
        removeItemData(ab->data);
      }
      delete ab;
      count--;
      return true;
    }
    current = current->next;
  }
  return false;
}

template <class T> bool DLinkedList<T>::contains(T item) {
  return indexOf(item) != -1;
}

template <class T> string DLinkedList<T>::toString(string (*item2str)(T &)) {
  stringstream chuoi;
  chuoi<< "[";
  Node *current = head->next;
  while (current != tail) {
    if (item2str)
      chuoi << item2str(current->data);
    else
      chuoi << current->data;
    current = current->next;
    if (current != tail) {
      chuoi << ", ";
    }
  }
  chuoi << "]";
  return chuoi.str();
}

template <class T> void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) {
  clear();
  Node *current = list.head->next;
  while (current != list.tail) {
    add(current->data);
    current = current->next;
  }
}

template <class T> void DLinkedList<T>::removeInternalData() {
  if (deleteUserData != nullptr) {
    deleteUserData(this);
  } else {
    clear();
  }
}

#endif /* DLINKEDLIST_H */

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "list/IList.h"
#include <iostream>
#include <memory.h>
#include <sstream>
#include <type_traits>
using namespace std;

template <class T> class XArrayList : public IList<T> {
public:
  class Iterator;

protected:
  T *data;
  int capacity;
  int count;
  bool (*itemEqual)(T &lhs, T &rhs); 
  void (*deleteUserData)(XArrayList<T> *); 

public:
  XArrayList(void (*deleteUserData)(XArrayList<T> *) = 0,
             bool (*itemEqual)(T &, T &) = 0, int capacity = 10);
  XArrayList(const XArrayList<T> &list);
  XArrayList<T> &operator=(const XArrayList<T> &list);
  ~XArrayList();
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
  // Inherit from IList: BEGIN
  void println(string (*item2str)(T &) = 0) {
    cout << toString(item2str) << endl;
  }

  void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0) {
    this->deleteUserData = deleteUserData;
  }

  Iterator begin() { return Iterator(this, 0); }

  Iterator end() { return Iterator(this, count); }

  static void free(XArrayList<T> *list) {
    typename XArrayList<T>::Iterator it = list->begin();
    while (it != list->end()) {
      delete *it;
      it++;
    }
  }

protected:
  void checkIndex(int index);     // check validity of index for accessing
  void ensureCapacity(int index); // auto-allocate if needed

  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

  void copyFrom(const XArrayList<T> &list);

  void removeInternalData();

  //////////////////////////////////////////////////////////////////////
  ////////////////////////  INNER CLASSES DEFNITION ////////////////////
  //////////////////////////////////////////////////////////////////////
public:
  // Iterator: BEGIN
  class Iterator {
  private:
    int cursor;
    XArrayList<T> *pList;

  public:
    Iterator(XArrayList<T> *pList = 0, int index = 0) {
      this->pList = pList;
      this->cursor = index;
    }

    Iterator &operator=(const Iterator &iterator) {
      cursor = iterator.cursor;
      pList = iterator.pList;
      return *this;
    }

    void remove(void (*removeItemData)(T) = 0) {
      T item = pList->removeAt(cursor);
      if (removeItemData != 0)
        removeItemData(item);
      cursor -= 1; // MUST keep index of previous, for ++ later
    }

    T &operator*() { return pList->data[cursor]; }

    bool operator!=(const Iterator &iterator) {
      return cursor != iterator.cursor;
    }
    // Prefix ++ overload
    Iterator &operator++() {
      this->cursor++;
      return *this;
    }
    // Postfix ++ overload
    Iterator operator++(int) {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }
  };
  // Iterator: END
};
////////////////////////     METHOD DEFNITION      ///////////////////
template <class T>
XArrayList<T>::XArrayList(void (*deleteUserData)(XArrayList<T> *),
                          bool (*itemEqual)(T &, T &), int capacity) {
  this->capacity = capacity;
  this->itemEqual = itemEqual;
  this->deleteUserData = deleteUserData;
  this->data = new T[capacity];
  this->count = 0;
}

template <class T> void XArrayList<T>::copyFrom(const XArrayList<T> &list) {
  if (this == &list) {
    return;
  }
  this->count = list.count;
  this->capacity = list.capacity;
  this->deleteUserData = list.deleteUserData;
  this->itemEqual = list.itemEqual;
  if (this->data != nullptr) {
    delete[] this->data;
  }
  if (list.data == nullptr) {
    this->data = nullptr;
    return;
  }
  this->data = new T[this->capacity];
  for (int i = 0; i < this->count; ++i) {
    this->data[i] = list.data[i];
  }
}

template <class T> void XArrayList<T>::removeInternalData() {
  if (this->deleteUserData) {
    this->deleteUserData(this);
  }
  if (this->data != nullptr) {
    delete[] this->data;
  }
  this->data = nullptr;
  this->count = 0;
  this->capacity = 0;
}

template <class T> XArrayList<T>::XArrayList(const XArrayList<T> &list) {
  if (this == &list) {
    return;
  }
  this->count = list.count;
  this->capacity = list.capacity;
  this->deleteUserData = list.deleteUserData;
  this->itemEqual = list.itemEqual;
  if (this->data != nullptr) {
    delete[] this->data;
  }
  if (list.data == nullptr) {
    this->data = nullptr;
    return;
  }
  this->data = new T[this->capacity];
  for (int i = 0; i < this->count; ++i) {
    this->data[i] = list.data[i];
  }
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list) {
  if (this == &list) {
    return *this;
  }
  removeInternalData();
  copyFrom(list);
  return *this;
}

template <class T> XArrayList<T>::~XArrayList() { removeInternalData(); }

template <class T> void XArrayList<T>::add(T e) {
  ensureCapacity(this->count);
  data[count] = e;
  count += 1;
}

template <class T> void XArrayList<T>::add(int index, T e) {
  
  if (index == this->count) {
    add(e);
    return;
  }
  checkIndex(index);
  ensureCapacity(count);
  for (int i = this->count; i > index; --i) {
    this->data[i] = this->data[i - 1];
  }
  this->data[index] = e;
  this->count += 1;
}

template <class T> T XArrayList<T>::removeAt(int index) {
  checkIndex(index);
  int i = index;
  T tmp = this->data[index];
  while (i < this->count - 1) {
    this->data[i] = this->data[i + 1];
    i += 1;
  }
  this->count--;
  return tmp;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T)) {
  int index = this->indexOf(item);
  if (index == -1) {
    return false;
  }
  if (removeItemData != nullptr) {
    removeItemData(this->data[index]);
  }
  this->removeAt(index);
  return true;
}

template <class T> bool XArrayList<T>::empty() {
   if(this->count == 0){ 
    return true; 
   }
   return false;
   }

template <class T> int XArrayList<T>::size() { return this->count; }

template <class T> void XArrayList<T>::clear() {
  if (this->data != nullptr) {
    delete[] this->data;
  }
  this->count = 0;
  this->data = new T[this->capacity];
}

template <class T> T &XArrayList<T>::get(int index) {
  checkIndex(index);
  return this->data[index];
}

template <class T> int XArrayList<T>::indexOf(T item) {
  for (int i = 0; i < count; i++) {
    if (equals(data[i], item, itemEqual)) {
      return i;
    }
  }
  return -1;
}

template <class T> bool XArrayList<T>::contains(T item) {
        return this->indexOf(item) !=-1 ;
}

template <class T> string XArrayList<T>::toString(string (*item2str)(T &)) {
  stringstream chuoi;
  chuoi << "[";
  for (int i = 0; i < count; ++i) {
    if (item2str)
      chuoi << item2str(data[i]);
    else
      chuoi << data[i];
    if (i < count - 1) {
      chuoi << ", ";
    }
  }
  chuoi << "]";
  return chuoi.str();
}

template <class T> void XArrayList<T>::checkIndex(int index) {
  if (index < 0 || index >= this->size()) {
    throw std::out_of_range("Index is out of range!");
  }
}

template <class T>
void XArrayList<T>::ensureCapacity(int index) {
    if (index < 0) {
        throw std::out_of_range("Index is out of range!");
    }

    if (capacity > index) {
        return;
    }

    int newCapacity = static_cast<int>(capacity * 1.5);

    if (newCapacity < index + 1) {
        newCapacity = index + 1;
    }

    T* tmp = nullptr;
    try {
        tmp = new T[newCapacity];
    }
    catch (const std::bad_alloc& e) {
        throw; 
    }

    for (int i = 0; i < count; ++i) {
        tmp[i] = data[i];
    }

    delete[] data;

    data = tmp;
    capacity = newCapacity;
}

#endif /* XARRAYLIST_H */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
class DLinkedListSE: public DLinkedList<T>{
public:
    
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*itemEQ)(T&, T&)=0 ) : 
            DLinkedList<T>(removeData, itemEQ){
        
    };
    
    DLinkedListSE(const DLinkedList<T>& list){
        this->copyFrom(list);
    }
    
    void sort(int (*comparator)(T&,T&)=0){
        if(this->count < 2) return;
        typename DLinkedList<T>::Node* tmp1 = this->tail->prev;
        tmp1->next = nullptr;
        typename DLinkedList<T>::Node* tmp2 = this->head->next;
        tmp2 = mergeSort(tmp2, comparator);
        this->head->next = tmp2;
        tmp2->prev = this->head;
        // Update tail and count
        typename DLinkedList<T>::Node* current = this->head;
        this->count = 0;
        while(current->next != nullptr){
            current = current->next;
            this->count++;
        }
        current->next = this->tail;
        this->tail->prev = current;
    }

private:
    typename DLinkedList<T>::Node* mergeSort(typename DLinkedList<T>::Node* head, int (*comparator)(T&, T&)){
        if (head == nullptr || head->next == nullptr) return head;

        typename DLinkedList<T>::Node* middle = getMiddle(head);
        typename DLinkedList<T>::Node* head2 = middle->next;

        middle->next = nullptr;
        if (head2 != nullptr) head2->prev = nullptr;

        typename DLinkedList<T>::Node* left = mergeSort(head, comparator);
        typename DLinkedList<T>::Node* right = mergeSort(head2, comparator);

        return merge(left, right, comparator);
    }

    typename DLinkedList<T>::Node* getMiddle(typename DLinkedList<T>::Node* head){
        if (head == nullptr) return head;
        typename DLinkedList<T>::Node* slow = head;
        typename DLinkedList<T>::Node* fast = head->next;

        while (fast != nullptr && fast->next != nullptr){
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;
    }

    typename DLinkedList<T>::Node* merge(typename DLinkedList<T>::Node* left, typename DLinkedList<T>::Node* right, int (*comparator)(T&, T&)){
        if (left == nullptr) return right;
        if (right == nullptr) return left;

        typename DLinkedList<T>::Node* result = nullptr;

        if (compare(left->data, right->data, comparator) <= 0){
            result = left;
            result->next = merge(left->next, right, comparator);
            if (result->next != nullptr) result->next->prev = result;
        } else {
            result = right;
            result->next = merge(left, right->next, comparator);
            if (result->next != nullptr) result->next->prev = result;
        }
        return result;
    }
protected:
    static int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)=0){
        if(comparator != 0) return comparator(lhs, rhs);
        else{
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
};
#endif 


#pragma once
#include "LinkedList.h"
template<typename T>
class queue {
public:
	int size = 0;
	LinkedList<T>* head, *tail;
	void add(LinkedList<T>* node) {
		if (tail == nullptr) {
			head = node;
			tail = node;
		} else {
			tail->next = node;
			tail = tail->next;
		}
		size++;
	}
	T pop() {
		T e = head->element;
		LinkedList<T>* n = head->next;
		delete head;
		if (head == tail) tail = nullptr;
		head = n;
		size--;
		return e;
	}
	~queue() {
		if (head == nullptr) return;
		LinkedList<T>* node = head, *temp;
		while (node != nullptr) {
			temp = node->next;
			delete node;
			node = temp;
		}
	}
};
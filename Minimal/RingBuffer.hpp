#pragma once
#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <vector>
#include <glm/glm.hpp>

template <class T> class RingBuffer
{
	unsigned int size; 
	unsigned int count; 
	int front; 
	int back; 
	T* items; 

public:
	RingBuffer(unsigned int size) {
		this->size = size; 
		front = 0; 
		back = 0;
		items = new T[size];
		count = 0; 
	}

	bool push(T item) {
		if (count == size) {
			return false; 
		}
		items[front] = item; 
		front = (front + 1) % size; 
		count++; 
		return true; 
	}

	T peek() {
		if (count == 0) {
			return T();
		}
		return items[back];
	}

	bool pop() {
		if (count == 0) {
			return false; 
		}
		back = (back + 1) % size; 
		count--; 
		return true; 
	}

	bool isEmpty() {
		return count == 0; 
	}

	int getCount() {
		return count; 
	}

	~RingBuffer() {
		delete [] items; 
		items = NULL; 
	}
};

#endif // RINGBUFFER_HPP

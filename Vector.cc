// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Rohit Ganesan

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector() {
	elements = NULL;
	count = 0;
	reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs) {
	reserved = rhs.reserved;								//assign variables
	count = rhs.count;
	elements = (T*)malloc(reserved * sizeof(T));			//allocate memory

	for (int i = 0; i < count; i++) {
		new(&elements[i])T(rhs.elements[i]);				//filling in the elements
	}
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
	if (this == &rhs) {										//if its pointing to itself
		return *this;
	}
	if (elements != NULL) {									//call Destructor if not NULL
		for (int i = 0; i < count; i++) {
			elements[i].~T();
		}
		free(elements);
	}
	reserved = rhs.reserved;
	count = rhs.count;

	elements = (T*)malloc(reserved * sizeof(T));			//Allocate memory

	for (int i = 0; i < count; i++) {
		new(&elements[i])T(rhs.elements[i]);				//filling the elements
	}

}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector() {
	if (elements) {											//Clear and free memory
		for (int i = 0; i < count; i++) {
			elements[i].~T();								//remove elements
		}
		free(elements);
	}
	reserved = 0; //resetting globals
	count = 0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs) {
	if (count == reserved) {
		reserved = reserved + 1;
		T* temp = (T*)malloc(reserved * sizeof(T));
		count = count + 1;

		for (int i = 0; i < (count-1); i++) {
			new(&temp[i])T(elements[i]);					//copy elements to temp
			elements[i].~T();								//delete old elements
		}

		new(temp+(count-1))T(rhs);							//new elements back to old position
		free(elements);
		elements = temp;									//copy temp back to elements
	} else {												//Count < reserve 
		count++;
		new(&elements[count-1])T(rhs);
	}
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs) {
	if (count == reserved) {
		reserved = reserved + 1;
		T* temp = (T*)malloc(reserved * sizeof(T));			//allocate memory
		count = count + 1;

		for (int i = 1; i < count; i++) {
			new(&temp[i])T(elements[i-1]);
			elements[i-1].~T();								//move old elements to temp
		}
		new(temp)T(rhs);
		free(elements);
		elements = temp;									//move back temp to elements
	} else {
		for (int i = count; i > count; i--) {
			new(&elements[i])T(elements[i-1]);
			elements[i-1].~T();
		}
		new(elements)T(rhs);
		count++;
	}
}

template <typename T>
void Vector<T>::Pop_Back() { // Remove last element
	if (count != 0) {
		count = count - 1;									//decrement count
		elements[count].~T();								//pop back
	}
}

template <typename T>
void Vector<T>::Pop_Front() { // Remove first element
	if (count != 0) {
		T* temp = (T*)malloc(reserved * sizeof(T));			//allocate memory
		for (int i = 1; i < count; i++) {
			new(&temp[i-1])T(elements[i]);
			elements[i].~T();
		}
		elements[0].~T();									//pop front
		free(elements);

		elements = temp;									//copy back temp to elements
		count = count - 1;									//decrement count
	}
}

// Element Access
template <typename T>
T& Vector<T>::Front() const {
	return elements[0];										//first element
}

// Element Access
template <typename T>
T& Vector<T>::Back() const {
	return elements[count-1];								//last element
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const { // const element access
	if (i < 0 || i >= count) {
		cout<<"Invalid Index"<<endl;
		abort();
	} else {
		return (*(elements + i));
	}
}	

template <typename T>
T& Vector<T>::operator[](size_t i) {//nonconst element access
	if (i < 0 || i >= count) {
		cout<<"Invalid Index"<<endl;
		abort();
	} else {
		return (*(elements + i));
	}
}

template <typename T>
size_t Vector<T>::Size() const {
	return count;
}

template <typename T>
bool Vector<T>::Empty() const {
	return (count == 0);
}

// Implement clear
template <typename T>
void Vector<T>::Clear() {
	for (int i = 0; i < count; i++) {
		elements[i].~T();										//destructing elements
	}
	count = 0;													//resetting count
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
	return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const {
	return VectorIterator<T>(elements + count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator() {
	current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c) {
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs) {
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const {
	return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++() {
	current = current + 1;
	return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int) {
	VectorIterator<T> previous = *this;
	current = current + 1;
	return previous;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const {
	return (current != rhs.current);
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const {
	return (current == rhs.current);
}





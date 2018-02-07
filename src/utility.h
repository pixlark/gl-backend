#ifndef STRATEGEM_UTILITY_H
#define STRATEGEM_UTILITY_H

#include <assert.h>

#include "stdafx.h"
//#define assert(condition) if(!condition) { fprintf(stderr, "Assertion Failed! %s:%d", __FILE__, __LINE__); exit(1); }

struct Vector2f;
struct Vector2i;
struct RGBA;

struct Vector2f {
	float x;
	float y;
	Vector2f()
		: x(0), y(0) {};
	Vector2f(float x, float y)
		: x(x), y(y) {};
	operator Vector2i();
};

struct Vector2i {
	int x;
	int y;
	Vector2i()
		: x(0), y(0) {};
	Vector2i(int x, int y)
		: x(x), y(y) {};
	operator Vector2f();
};

struct RGBA {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	RGBA()
		: r(0), g(0), b(0), a(0) {};
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a) {};
};

template <typename T>
struct List {
	T * arr;
	int len;
	int len_max;
	
	void alloc();
	void dealloc();
	List<T> copy();
	void push(T p);
	T pop();
	T& operator[](int indice);
	int remove(int indice);
	
	void _grow();
	void _shrink();
};

template <typename T>
void List<T>::alloc()
{
	len = 0;
	len_max = 4;
	arr = (T*) malloc(sizeof(T) * len_max);
}

template <typename T>
void List<T>::dealloc()
{
	free(arr);
}

template <typename T>
List<T> List<T>::copy()
{
	List<T> ret;
	ret.arr = (T*) malloc(sizeof(T) * len_max);
	ret.len = len;
	ret.len_max = len_max;
	for (int i = 0; i < len; i++) {
		ret.arr[i] = arr[i];
	}
	return ret;
}

template <typename T>
void List<T>::push(T p)
{
	if (len >= len_max) {
		_grow();
	}
	arr[len++] = p;
}

template <typename T>
T List<T>::pop()
{
	assert(len > 0);
	T ret = arr[--len];
	if (len <= len_max / 4 && len_max > 4) {
		_shrink();
	}
	return ret;
}

template <typename T>
T& List<T>::operator[](int indice)
{
	return arr[indice];
}

template <typename T>
int List<T>::remove(int indice)
{
	assert(indice < len);
	assert(len > 0);	
	for (int i = indice; i < len - 1; i++) {
		arr[i] = arr[i + 1];
	}
	len--;
	if (len <= len_max / 4 && len_max > 4) {
		_shrink();
	}
	return 0;
}

template <typename T>
void List<T>::_grow()
{
	T * new_arr = (T*) malloc(sizeof(T) * len_max * 2);
	for (int i = 0; i < len_max; i++) {
		new_arr[i] = arr[i];
	}
	free(arr);
	len_max *= 2;
	arr = new_arr;
}

template <typename T>
void List<T>::_shrink()
{
	T * new_arr = (T*) malloc(sizeof(T) * (len_max / 2));
	for (int i = 0; i < (len_max / 2); i++) {
		new_arr[i] = arr[i];
	}
	free(arr);
	len_max /= 2;
	arr = new_arr;
}

#endif

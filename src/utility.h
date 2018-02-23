#ifndef _UTILITY_H
#define _UTILITY_H

#include <assert.h>
#include <stdint.h>
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
	Vector2f &operator+=(Vector2f &r);
	Vector2f operator+(Vector2f o);
	Vector2f &operator-=(Vector2f &r);
	Vector2f operator-(Vector2f o);
	bool operator==(Vector2f o);
};

struct Vector2i {
	int x;
	int y;
	Vector2i()
		: x(0), y(0) {};
	Vector2i(int x, int y)
		: x(x), y(y) {};
	operator Vector2f();
	Vector2i &operator+=(Vector2i &r);
	Vector2i operator+(Vector2i o);
	Vector2i &operator-=(Vector2i &r);
	Vector2i operator-(Vector2i o);
	bool operator==(Vector2i o);
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
	bool allocated = false;
	
	void alloc();
	void dealloc();
	List<T> copy() const;
	void push(T p);
	void cat(T * p, int size, int offset = 0);
	T pop();
	int find(T s, bool(equality)(T, T)) const;
	int swap(int a, int b);
	void reverse();
	T& operator[](int indice) const;
	int remove(int indice);
	
	void _grow();
	void _shrink();
};

template <typename T>
void List<T>::alloc()
{
	if (allocated) return; // TODO(pixlark): Should this be silent?
	len = 0;
	len_max = 4;
	arr = (T*) malloc(sizeof(T) * len_max);
	allocated = true;
}

template <typename T>
void List<T>::dealloc()
{
	if (!allocated) return;
	free(arr);
	allocated = false;
}

template <typename T>
List<T> List<T>::copy()
const
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
void List<T>::cat(T * p, int size, int offset)
{
	for (int i = 0; i < offset; i++) {
		if (len == 0) break;
		pop();
	}
	while (len + size >= len_max) {
		_grow();
	}
	for (int i = 0; i < size; i++) {
		arr[len++] = p[i];
	}
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
int List<T>::find(T s, bool(equality)(T, T))
const
{
	for (int i = 0; i < len; i++) {
		if (equality(arr[i], s)) {
			return i;
		}
	}
	return -1;
}

template <typename T>
int List<T>::swap(int i, int j)
{
	if (i < 0 || i >= len || j < 0 || j >= len) {
		return -1;
	}
	T a = arr[i];
	arr[i] = arr[j];
	arr[j] = a;
	return 0;
}

template <typename T>
void List<T>::reverse()
{
	int i = 0;
	int j = len - 1;
	while (i < j) {
		swap(i++, j--);
	}
}

template <typename T>
T& List<T>::operator[](int indice)
const
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

void os_path(char * path, int len);
List<char> get_exe_dir();

#endif

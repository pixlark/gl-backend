#include "utility.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <unistd.h>
#endif

void os_path(char * path, int len)
{
	for (int i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') {
#if defined(_WIN32) || defined(_WIN64)
			path[i] = '\\';
#else
			path[i] = '/';
#endif	
		}
	}
}

List<char> get_exe_dir()
{
	List<char> path;
	path.alloc();
	char * buffer = (char*) malloc(512);
#if defined(_WIN32) || defined(_WIN64)
	int len = GetModuleFileName(NULL, buffer, 512) + 1;
#else
	// TODO(pixlark): Test this on nix
	int len = readlink("/proc/self/exe", buffer, 511) + 1;
	assert(len != -1);
	buffer[len - 1] = '\0';
#endif
	os_path(buffer, len);
	// TODO(pixlark): Get rid of this hacky string.h bullshit
	//                and write an actual string builder
#if defined(_WIN32) || defined(_WIN64)
	char * s = strrchr(buffer, '\\');
#else
	char * s = strrchr(buffer, '/');
#endif
	*(s + 1) = '\0';
	path.cat(buffer, strlen(buffer) + 1, 1);
	free(buffer);
	return path;
}

Vector2f::operator Vector2i()
{
	return Vector2i((int) x, (int) y);
}

Vector2i::operator Vector2f()
{
	return Vector2f((float) x, (float) y);
}

Vector2f &Vector2f::operator+=(Vector2f &r)
{
	x += r.x;
	y += r.y;
	return *this;
}

Vector2i &Vector2i::operator+=(Vector2i &r)
{
	x += r.x;
	y += r.y;
	return *this;
}

Vector2f Vector2f::operator+(Vector2f o)
{
	Vector2f ret = *this;
	ret += o;
	return ret;
}

Vector2i Vector2i::operator+(Vector2i o)
{
	Vector2i ret = *this;
	ret += o;
	return ret;
}

Vector2i &Vector2i::operator-=(Vector2i &r)
{
	x -= r.x;
	y -= r.y;
	return *this;
}

Vector2f &Vector2f::operator-=(Vector2f &r)
{
	x -= r.x;
	y -= r.y;
	return *this;
}

Vector2i Vector2i::operator-(Vector2i o)
{
	Vector2i ret = *this;
	ret -= o;
	return ret;
}

Vector2f Vector2f::operator-(Vector2f o)
{
	Vector2f ret = *this;
	ret -= o;
	return ret;
}

bool Vector2f::operator==(Vector2f o)
{
	return (x == o.x && y == o.y);
}

bool Vector2i::operator==(Vector2i o)
{
	return (x == o.x && y == o.y);
}

#include "utility.h"

/*
 * Utility Classes and Functions
 */

Vector2f::operator Vector2i()
{
	return Vector2i((int) x, (int) y);
}

Vector2i::operator Vector2f()
{
	return Vector2f((float) x, (float) y);
}


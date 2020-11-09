#pragma once
#include "../src/types.h"
#include <raylib.h>
#include <math.h>

template <class T>
struct Vector2d {

	inline Vector2d() : x(0), y(0) {};
	inline Vector2d(T x, T y) : x(x), y(y) {};
	

	T x, y;

	operator Vector2() const;
	operator bool()    const;
	// operator Vector2d<T>() const;

	bool operator ==(const Vector2d &rhs)  const;

	inline Vector2d(Vector2 vec) : x(vec.x), y(vec.y) {} ;

	Vector2d operator+= (const Vector2d &rhs); 
	Vector2d operator+  (const Vector2d &rhs);

	Vector2d operator-= (const Vector2d &rhs); 
	Vector2d operator-  (const Vector2d &rhs);
	Vector2d operator-  ();

	Vector2d operator*= (const Vector2d &rhs);
	Vector2d operator*  (const Vector2d &rhs);
	Vector2d operator*= (T rhs);
	Vector2d operator*  (T rhs);

	Vector2d operator/= (const Vector2d &rhs);
	Vector2d operator/  (const Vector2d &rhs);
	Vector2d operator/= (T rhs);
	Vector2d operator/  (T rhs);

	Vector2d floor();
	Vector2d ceil();
	Vector2d round();

	T mag();
	T mag2();

	Vector2d norm();
};

typedef Vector2d<f32> v2f;
typedef Vector2d<s32> v2i;
typedef Vector2d<u8>  v2u8;

Vector2d<s32> Vec2fToVec2i(Vector2d<s32> *out, const Vector2d<f32> &in);
Vector2d<f32> Vec2iToVec2f(Vector2d<f32> *out, const Vector2d<s32> &in);

#include "Vector2.h"

template class Vector2d<f32>;
template class Vector2d<s32>;
template class Vector2d<u8>;

#ifdef raylib
template <class T>
Vector2d<T>::operator Vector2() const {
	Vector2 out;
	out.x = this->x;
	out.y = this->y;
	return out;
}
#endif

template <class T>
Vector2d<T>::operator bool() const {
	bool out = (this->x || this->y);
	return out;
}

// template <class T>
// Vector2d<T>::operator Vector2d<T>() const {
// 	Vector2d<T> out;
// 	out.x = (T) this->x;
// 	out.x = (T) this->y;
// 	return out;
// }

template <class T>
bool Vector2d<T>::operator==(const Vector2d<T> &rhs) const {
	return ((this->x == rhs.x) && (this->y == rhs.y));
}

template <class T>
Vector2d<T> Vector2d<T>::operator+=(const Vector2d<T> &rhs) {
    this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

template <class T>
Vector2d<T> Vector2d<T>::operator+(const Vector2d<T> &rhs) {
    return { this->x + rhs.x, 
	       this->y + rhs.y };
}

template <class T>
Vector2d<T> Vector2d<T>::operator-=(const Vector2d<T> &rhs) {
    this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

template <class T>
Vector2d<T> Vector2d<T>::operator-(const Vector2d<T> &rhs) {
    return { this->x -rhs.x, 
	       this->y - rhs.y };
}

template <class T>
Vector2d<T> Vector2d<T>::operator-() {
    return { -this->x, 
	       -this->y };
}

template <class T>
Vector2d<T> Vector2d<T>::operator*=(const Vector2d<T> &rhs) {
    this->x *= rhs.x;
	this->y *= rhs.y;
	return *this;
}

template <class T>
Vector2d<T> Vector2d<T>::operator*=(T rhs) {
	return { this->x *= rhs,
	 this->y *= rhs };
}

template <class T>
Vector2d<T> Vector2d<T>::operator*(const Vector2d<T> &rhs) {
    return { this->x * rhs.x, 
	       this->y * rhs.y };
}

template <class T>
Vector2d<T> Vector2d<T>::operator*(T rhs) {
    return { this->x * rhs, 
	       this->y * rhs };
}

template <class T>
Vector2d<T> Vector2d<T>::operator/=(const Vector2d<T> &rhs) {
    this->x /= rhs.x;
	this->y /= rhs.y;
	return *this;
}

template <class T>
Vector2d<T> Vector2d<T>::operator/=(T rhs) {
    return { this->x /= rhs, 
	         this->y /= rhs };
}

template <class T>
Vector2d<T> Vector2d<T>::operator/(const Vector2d<T> &rhs) {
    return { this->x / rhs.x, 
	         this->y / rhs.y };
}

template <class T>
Vector2d<T> Vector2d<T>::operator/(T rhs) {
    return { this->x / rhs, 
	       this->y / rhs };
}

template <class T>
Vector2d<T> Vector2d<T>::floor() {
    return { (T) floorf(this->x), 
	       (T) floorf(this->y)};
}

template <class T>
Vector2d<T> Vector2d<T>::ceil() {
    return { (T) ceilf(this->x), 
	       (T) ceilf(this->y)};
}

template <class T>
T Vector2d<T>::mag() {
    return sqrtf((this->x * this->x) + (this->y * this->y));
}


template <class T>
T Vector2d<T>::mag2() {
    return ((this->x * this->x) + (this->y * this->y));
}


template <class T>
Vector2d<T> Vector2d<T>::norm() {
	T temp = this->mag();
	return ((*this) /= temp);
}



#include "Vector2.h"

template class Vector2d<f32>;
template class Vector2d<s32>;

template <class T>
Vector2d<T>::operator Vector2() const {
	Vector2 out;
	out.x = this->x;
	out.y = this->y;
	return out;
}

template <class T>
Vector2d<T>::operator bool() const {
	bool out;
	out = (this->x || this->y);
	return out;
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
    return { (T) floorf64(this->x), 
	       (T) floorf64(this->y)};
}

template <class T>
Vector2d<T> Vector2d<T>::ceil() {
    return { (T) ceilf64(this->x), 
	       (T) ceilf64(this->y)};
}

template <class T>
T Vector2d<T>::mag() {
    return sqrtf64((this->x * this->x) + (this->y * this->y));
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



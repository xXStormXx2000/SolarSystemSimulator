#include "Vector3D.h"

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {};

Vector3D Vector3D::operator*(double num) {
	return Vector3D(x * num, y * num, z * num);
}

Vector3D& Vector3D::operator*=(double num) {
	*this = *this * num;
	return *this;
}

Vector3D Vector3D::operator/(double num) {
	return Vector3D(x / num, y / num, z / num);
}

Vector3D& Vector3D::operator/=(double num) {
	*this = *this / num;
	return *this;
}

double Vector3D::abs() {
	return sqrt(x*x+y*y+z*z);
}

double Vector3D::norm() {
	return abs();
}

double Vector3D::distance(const Vector3D& other) {
	return (*this - other).abs();
}

Vector3D Vector3D::operator+(Vector3D other) {
	other.x += x;
	other.y += y;
	other.z += z;
	return other;
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
	*this = *this + other;
	return *this;
}
Vector3D Vector3D::operator-(Vector3D other) const {
	other.x = x - other.x;
	other.y = y - other.y;
	other.z = z - other.z;
	return other;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
	*this = *this - other;
	return *this;
}

Vector3D Vector3D::unitVector() {
	return (*this / abs());
}

void Vector3D::normalize() {
	*this = this->unitVector();
}

double Vector3D::operator*(const Vector3D& other) {
	return this->x*other.x + this->y * other.y + this->z * other.z;
}

Vector3D Vector3D::crossProduct(const Vector3D& other) {
	Vector3D out;
	out.x = y * other.z - z * other.y;
	out.y = z * other.x - x * other.z;
	out.z = x * other.y - y * other.x;
	return out;
}

std::string Vector3D::toString() {
	return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
}

#pragma once
#include <math.h>
#include <string>
class Vector3D {
public:
	double x, y, z;

	Vector3D(double x = 0, double y = 0, double z = 0);
	Vector3D operator*(double num);
	Vector3D& operator*=(double num);
	Vector3D operator/(double num);
	Vector3D& operator/=(double num);
	double abs();
	double norm();
	double distance(const Vector3D& other);
	Vector3D operator+(Vector3D other);
	Vector3D& operator+=(const Vector3D& other);
	Vector3D operator-(Vector3D other);
	Vector3D& operator-=(const Vector3D& other);
	Vector3D unitVector();
	void normalize();
	double operator * (const Vector3D& other);
	Vector3D crossProduct(const Vector3D& other);
	std::string toString();
};


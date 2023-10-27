#pragma once
#include <string>
#include "Vector3D.h"
#include <stdexcept>
#include <vector>

class celestialBody {
private:
	//Big G
	double G = 6.6743e-11;

	//Mass in kilograms
	double mass;

	//Radius in meters
	double radius;

	Vector3D singleBodyGravitationalForce(const celestialBody&);

	std::vector<int> color = {255, 255, 255, 255};

public:
	std::string name;

	//Velocity is in m/s
	Vector3D velocity;

	//Position in meters
	Vector3D position;

	celestialBody(std::string, double, double, Vector3D, Vector3D, std::vector<int>);
	void setMass(double);
	double getMass();
	void setRadius(double);
	double getRadius();
	void setColor(int r, int g, int b, int a);
	std::vector<int> getColor();

	//Acceleration due to gravity per second
	Vector3D accelerationDueToGravity(const std::vector<celestialBody*>&);

	//Resolve collision
	double resolveCollision(const std::vector<celestialBody*>&);
};


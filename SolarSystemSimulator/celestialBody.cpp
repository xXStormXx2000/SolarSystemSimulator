#include "celestialBody.h"

Vector3D celestialBody::singleBodyGravitationalForce(const celestialBody& other) {
	double d = position.distance(other.position);
	return ((position - other.position) * other.mass)/(d * d * d);
}

celestialBody::celestialBody(std::string name, double mass, double radius, Vector3D pos, Vector3D vel, std::vector<int> color) : name(name), velocity(vel), position(pos) {
	setMass(mass);
	setRadius(radius);
	setColor(color[0], color[1], color[2], color[3]);
}

void celestialBody::setMass(double num) {
	if (num < 0) throw std::runtime_error("Mass can not be negative");
	mass = num;
}

double celestialBody::getMass() {
	return mass;
}

void celestialBody::setRadius(double num) {
	if (num < 0) throw std::runtime_error("Radius can not be negative");
	radius = num;
}

double celestialBody::getRadius() {
	return radius;
}

void celestialBody::setColor(int r, int g, int b, int a) {
	if(!(0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255 && 0 <= a && a <= 255)) throw std::runtime_error("Values must be between 0 and 255");
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

std::vector<int> celestialBody::getColor() {
	return color;
}

Vector3D celestialBody::accelerationDueToGravity(const std::vector<celestialBody*>& solarSystem) {
	Vector3D gravityAcceleration;
	for (const celestialBody* i : solarSystem) {
		if (i == this) continue;
		gravityAcceleration += singleBodyGravitationalForce(*i);
	}
	gravityAcceleration *= G;
	return gravityAcceleration;
}


//Resolve Collision
void celestialBody::resolveCollision(const std::vector<celestialBody*>& solarSystem) {
	celestialBody* other = nullptr;
	for (celestialBody* i : solarSystem) {
		if (i == this) continue;
		if (radius + i->radius > position.distance(i->position)) {
			other = i;
			break;
		}
	}
	if (other == nullptr) return;
	position = ((position - other->position).unitVector())*(radius+other->radius) + other->position;
	velocity = other->velocity;
}

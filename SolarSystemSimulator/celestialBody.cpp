#include "celestialBody.h"

Vector3D celestialBody::singleBodyGravitationalForce(const celestialBody& other) {
	double d = position.distance(other.position);
	return ((other.position - position) * other.mass)/(d * d * d);
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
double celestialBody::resolveCollision(const std::vector<celestialBody*>& solarSystem) {
	celestialBody* other = nullptr;
	double t1 = 0, r = 0, sq = 0;
	Vector3D p, v;
	for (celestialBody* i : solarSystem) {
		if (i == this) continue;
		v = i->velocity - velocity;
		if (v * v == 0) continue;
		p = i->position - position;
		r = i->radius + radius;
		if ((v * p) * (v * p) - (v * v) * ((p * p) - r * r) < 0) continue;
		sq = sqrt((v * p) * (v * p) - (v * v) * ((p * p) - r * r));
		t1 = -(v * p + sq) / (v * v);
		if (-0.001 <= t1 && t1 <= 1) {
			other = i;
			break;
		}
	}
	if (other == nullptr) return false;
	double t2 = 1 - t1;

	position += velocity * t1;
	other->position += other->velocity * t1;

	Vector3D axisVec = (other->position - position).unitVector();
	Vector3D v1 = axisVec * (velocity * axisVec);
	Vector3D v2 = axisVec * (other->velocity * axisVec);

	velocity += (v1 * (mass - other->mass) + v2*2*other->mass) / (mass + other->mass) - v1;
	other->velocity += (v1 * 2 * mass + v2 * (other->mass - mass)) / (mass + other->mass) - v2;

	position += velocity * t2;
	other->position += other->velocity * t2;

	return t1;
}

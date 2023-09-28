#include "stdafx.h"
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Given:
// spatial system: x is left-right axis, y is forward-backward axis, z is up-down axis
// any curve is centered at (0,0,0)

#define pi 3.1416

struct point3D {
	struct { double x, y, z; };
};


class Curve {
public:
	virtual point3D			point(double t) = 0;
	virtual vector<double>	deriv(double t) = 0;
	Curve() {};

	void print(double t) {
		point3D p = this->point(t);
		cout << fixed << setprecision(2) << "Point:\t\t" << p.x << " " << p.y << " " << p.z << endl;
		vector<double> vec = this->deriv(t);
		cout << "1st deriv:\t";
		for each (double d in vec) {
			cout << d << " ";
		}
		cout << endl;
	}
};

class Circle : public Curve {
private:
public:
	// should be private but just to speed up the process
	double radius;
	Circle(double radius = 1) {
		this->radius = radius > 0 ? radius : -radius;
	}
	point3D point(double t) {
		double te = t;
		if (te < 0) te = -te;
		while (te > 2*pi) te -= 2*pi;

		point3D p;
		p.x = radius * cos(te); 
		p.y = radius * sin(te); 
		p.z = 0;
		return p;
	}
	vector<double> deriv(double t) {
		double te = t;
		if (te < 0) te = -te;
		while (te > 2*pi) te -= 2*pi;

		// x^2+y^2=1 => x' = -y/x; y' = -x/y; z' = 0;
		vector<double> vec;
		point3D p = this->point(te);
		vec.push_back(-p.y / p.x); vec.push_back(-p.x / p.y); vec.push_back(0);
		return vec;
	}

	bool operator<(Circle const c1) {
		return this->radius < c1.radius;
	}
	bool operator<=(Circle const c1) {
		return this->radius <= c1.radius;
	}
	bool operator>(Circle const c1) {
		return this->radius > c1.radius;
	}
	bool operator>=(Circle const c1) {
		return this->radius >= c1.radius;
	}
	bool operator==(Circle const c1) {
		return this->radius == c1.radius;
	}
};

class Ellipse : public Curve {
private:
	//x axis radius (a)
	double x_rad;
	//y axis radius (b)
	double y_rad;
public:
	Ellipse(double rad1 = 1, double rad2 = 1.5) {
		this->x_rad = rad1 > 0 ? rad1 : -rad1;
		this->y_rad = rad2 > 0 ? rad2 : -rad2;
	}
	point3D point(double t) {
		double te = t;
		if (te < 0) te = -te;
		while (te > 2*pi) te -= 2*pi;

		point3D p;
		p.x = x_rad * cos(te);
		p.y = y_rad * sin(te);
		p.z = 0;
		return p;
	}
	vector<double> deriv(double t) {
		double te = t;
		if (te < 0) te = -te;
		while (te > pi) te -= pi;

		// x^2/a^2+y^2/b^2=1 => x' = -ya^2/xb^2; y' = -xb^2/ya^2; z' = 0;
		vector<double> vec;
		point3D p = this->point(te);
		vec.push_back((-p.y*x_rad*x_rad) / (p.x*y_rad*y_rad)); vec.push_back((-p.x*y_rad*y_rad) / (p.y*x_rad*x_rad)); vec.push_back(0);
		return vec;
	}
};

class Helix : public Curve {
private:
	double radius;
	//height difference between the start of each round
	double step;
public:
	Helix(double radius = 1, double step = 3) {
		this->radius = radius > 0 ? radius : -radius;
		this->step = step;
	}
	point3D point(double t) {
		double te = t;
		if (te < 0) te = -te;

		point3D p;
		p.x = radius * cos(te);
		p.y = radius * sin(te);
		p.z = (te / (pi * 2)) * step;
		return p;
	}
	vector<double> deriv(double t) {
		double te = t;
		if (te < 0) te = -te;

		// x^2+y^2=1 => x' = -y/x; y' = -x/y; z' = 0;
		vector<double> vec;
		point3D p = this->point(te);
		vec.push_back(-p.y / p.x); vec.push_back(-p.x / p.y); vec.push_back(step);
		return vec;
	}
};

bool mySorter(Circle* a, Circle* b) { return *a < *b; }
/*template<class T>
bool mySorter(T* a, T* b) { return *a > *b; }*/

int main()
{
	// Task 2
	srand(time(0));
	vector<Curve*> v1;
	for (int i = 0; i < 10; i++) {
		int pick1 = rand() % 3 + 1;
		double pick2 = 10 * rand() / (double)RAND_MAX + 1;
		double pick3 = 10 * rand() / (double)RAND_MAX + 1;
		switch (pick1) {
			case 1: 
				v1.push_back(new Circle(pick2));
				break;
			case 2: 
				v1.push_back(new Ellipse(pick2, pick3));
				break;
			case 3: 
				v1.push_back(new Helix(pick2, pick3));
				break;
		}
	}
	// Task 3
	for each (Curve* c in v1) {
		c->print(pi / 4);
		cout << endl;
	}
	// Task 4
	vector<Circle*> v2;
	for each (Curve* c in v1) {
		if (typeid(*c) == typeid(Circle))
			v2.push_back((Circle*)c);
	}
	// Task 5
	sort(v2.begin(), v2.end(), mySorter);
	// Task 6
	double sum = 0;
	for each (Circle* c in v2) sum += c->radius;

	getchar();
    return 0;
}

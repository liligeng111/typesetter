#pragma once

class Vector
{
public:
	Vector(float x, float y) : x_(x), y_(y), z_(1) {}
	Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}
	~Vector();

	float x() const { return x_; }
	float y() const { return y_; }
	float z() const { return z_; }

private:
	float x_, y_, z_;
};


class Matrix
{
public:
	Matrix();
	~Matrix();
	
private:
	float m_[][];
};

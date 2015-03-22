#pragma once

class Vector3l
{
public:
	Vector3l() : x_(1), y_(1), z_(1) {}
	Vector3l(long x, long y) : x_(x), y_(y), z_(1) {}
	Vector3l(long x, long y, long z) : x_(x), y_(y), z_(z) {}
	~Vector3l();

	int x() const { return x_; }
	int y() const { return y_; }
	int z() const { return z_; }

private:
	long x_, y_, z_;
};


class Matrix
{
public:
	Matrix();
	~Matrix();

	void Translate(float x, float y);
	
private:
	//float m_[][];
};

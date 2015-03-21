#pragma once
class Viewport
{
public:
	Viewport(float x_min, float y_min, float x_max, float y_max);
	~Viewport();

	int x_min() const { return x_min_; }
	int y_min() const { return y_min_; }
	int x_max() const { return x_max_; }
	int y_max() const { return y_max_; }

private:
	int x_min_;
	int y_min_;
	int x_max_;
	int y_max_;
};


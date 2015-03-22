#include "algebra.h"


Vector3l::~Vector3l()
{
}


Matrix::Matrix()
{
	float m_[3][3];

	//identity matrix
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j)
				m_[i][j] = 1;
			else
				m_[i][j] = 0;
		}
	}
}


Matrix::~Matrix()
{
}

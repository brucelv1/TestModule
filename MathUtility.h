#pragma once

#include "complex.hpp"
#define DOUBLE_PI    6.283185307179586476925286766559

namespace MathUtility
{
	void Burg_AR(double *x,int N,int p,double *a);

	// ���ٸ���Ҷ�任
	// data ����Ϊ (2 * 2^n), data ��żλΪʵ������, data ����λΪ��������
	// isInverse��ʾ�Ƿ�Ϊ��任
	void FFT(double * data, int n, bool isInverse = false);
}


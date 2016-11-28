#pragma once

#include "complex.hpp"
#define DOUBLE_PI    6.283185307179586476925286766559

namespace MathUtility
{
	void Burg_AR(double *x,int N,int p,double *a);

	// 快速傅里叶变换
	// data 长度为 (2 * 2^n), data 的偶位为实数部分, data 的奇位为虚数部分
	// isInverse表示是否为逆变换
	void FFT(double * data, int n, bool isInverse = false);
}


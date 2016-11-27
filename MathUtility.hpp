#pragma once

#include "complex.hpp"
#define DOUBLE_PI    6.283185307179586476925286766559

namespace MathUtility
{
	void Burg_AR(double *x,int N,int p,double *a)
	{   
		int i,k;   
		double R0,sumd,sumn,*b,*ef,*eb,*garma,*sigma2;   
		b=(double*)malloc((p+1)*sizeof(double));   
		ef=(double*)malloc(N*sizeof(double));   
		eb=(double*)malloc(N*sizeof(double));   
		garma=(double*)malloc((p+1)*sizeof(double));
		sigma2=(double*)malloc((p+1)*sizeof(double));   

		//��ʼ��,0��ARģ��   
		a[0]=1.0;   
		b[0]=1.0;   
		for (i=1;i<=p;i++)   
		{   
			a[i]=b[i]=0;   
		}   

		R0=0.0;      
		for   (i=0;i<N;i++)   
		{   
			R0+=x[i]*x[i]/N;   
		}   
		sigma2[0]=R0;   

		for (i=0;i<N;i++)   
		{   
			ef[i]=x[i];   
			eb[i]=x[i];   
		}   

		//��׼���   

		for (k=1;k<=p;k++)   
		{   
			sumn=0.0;   
			sumd=0.0;   
			for (i=k;i<N;i++)   
			{   
				sumn+=ef[i]*eb[i-1];   
				sumd+=ef[i]*ef[i]+eb[i-1]*eb[i-1];   
			}   
			garma[k]=-2*sumn/sumd;   

			for (i=1;i<=k;i++)   
			{   
				b[i]=a[i]+garma[k]*a[k-i];//�����µ�k��ARϵ��   
			}   
			for (i=1;i<=k;i++)   
			{   
				a[i]=b[i];//�µ�k��ARϵ����ֵ   
			}   

			sigma2[k]=(1.0-garma[k]*garma[k])*sigma2[k-1];//����������   

			//���㱾��ARģ�͵�ǰ��ͺ���Ԥ�����   
			//�Ӻ���ǰ������Խ�Լ�洢�ռ�   
			for (i=(N-1);i>=k;i--)   
			{   
				eb[i]=eb[i-1]+garma[k]*ef[i];   
				ef[i]=ef[i]+garma[k]*eb[i-1];      
			}   
		}   

		free(b);   
		free(ef);   
		free(eb);   
		free(garma);
		free(sigma2);
	}

	// ���ٸ���Ҷ�任
	// data ����Ϊ (2 * 2^n), data ��żλΪʵ������, data ����λΪ��������
	// isInverse��ʾ�Ƿ�Ϊ��任
	void FFT(double * data, int n, bool isInverse = false)
	{
		int mmax, m, j, step, i;
		double temp;
		double theta, sin_htheta, sin_theta, pwr, wr, wi, tempr, tempi;
		n = 2 * (1 << n);
		int nn = n >> 1;
		// ����Ϊ1�ĸ���Ҷ�任, λ�ý�������
		j = 1;
		for(i = 1; i < n; i += 2)
		{
			if(j > i)
			{
				temp = data[j - 1];
				data[j - 1] = data[i - 1];
				data[i - 1] = temp;
				data[j] = temp;
				data[j] = data[i];
				data[i] = temp;
			}
			// �෴�Ķ����Ƽӷ�
			m = nn;
			while(m >= 2 && j > m)
			{
				j -= m;
				m >>= 1;
			}
			j += m;
		}
		// Danielson - Lanczos ����Ӧ��
		mmax = 2;
		while(n > mmax)
		{
			step = mmax << 1;
			theta = DOUBLE_PI / mmax;
			if(isInverse)
			{
				theta = -theta;
			}
			sin_htheta = sin(0.5 * theta);
			sin_theta = sin(theta);
			pwr = -2.0 * sin_htheta * sin_htheta;
			wr = 1.0;
			wi = 0.0;
			for(m = 1; m < mmax; m += 2)
			{
				for(i = m; i <= n; i += step)
				{
					j = i + mmax;
					tempr = wr * data[j - 1] - wi * data[j];
					tempi = wr * data[j] + wi * data[j - 1];
					data[j - 1] = data[i - 1] - tempr;
					data[j] = data[i] - tempi;
					data[i - 1] += tempr;
					data[i] += tempi;
				}
				sin_htheta = wr;
				wr = sin_htheta * pwr - wi * sin_theta + wr;
				wi = wi * pwr + sin_htheta * sin_theta + wi;
			}
			mmax = step;
		}
	}
}


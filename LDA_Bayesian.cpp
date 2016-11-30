#include "LDA_Bayesian.h"
#include <iostream>
#include <fstream>
#include "eigen-3.2.1/Eigen/Dense"

LDA_Bayesian::LDA_Bayesian()
{
	_mFeaWinWidth = 200;
	_mFeaDimension = 32;
	_mChannelNum = 8;
}
LDA_Bayesian::~LDA_Bayesian()
{

}

bool LDA_Bayesian::Bayestrain(vector<vector<double> >& feature, vector<int>& label)
{
	using namespace Eigen;  

	//number of features and dimension of features
	int feat_num = feature.size();
	int feat_dim = feature[0].size();
	//number of labels
	int lab_num = label.size();

	if(lab_num!=feat_num)
	{
		std::cout << "the number of features is not equal to the number of labels"<< std::endl;
		return false;
	}
	//unique label of labels
	_mClassLabel = label;
	sort(_mClassLabel.begin(), _mClassLabel.end());
	_mClassLabel.erase(unique(_mClassLabel.begin(), _mClassLabel.end()), _mClassLabel.end());

	//Eigen matrix class
	MatrixXd featmat(feat_num,feat_dim);
	for(int i=0;i<feat_num;i++)
	{
		for (int j=0;j<feat_dim;j++)
		{
			featmat(i,j)=feature[i][j];
		}
	}
	//number of classes
	int cnum = _mClassLabel.size();
	// mean matrix and covariance matrix for each class
	MatrixXd MeanMat = MatrixXd::Zero(cnum,feat_dim);
	MatrixXd CovMat = MatrixXd::Zero(feat_dim*cnum,feat_dim);
	// pool covariance matrix
	MatrixXd PoolCovMat = MatrixXd::Zero(feat_dim,feat_dim);
	// sample number for each class
	VectorXd feat_num_perclass = VectorXd::Zero(cnum);
	// compute the mean vector for each class
	for(int i=0;i<feat_num;i++)
	{
		for(int j=0;j<cnum;j++)
		{
			if(label[i]==_mClassLabel[j])
			{
				MeanMat.row(j)=MeanMat.row(j)+ featmat.row(i);
				feat_num_perclass(j) = feat_num_perclass(j)+1;
			}
		}
	}
	for(int i=0;i<cnum;i++)
	{
		MeanMat.row(i)=MeanMat.row(i)/feat_num_perclass(i);
	}
	//compute the covariance matrix for each class and pool covariance matrix
	for(int i=0;i<feat_num;i++)
	{
		for(int j=0;j<cnum;j++)
		{
			if(label[i]==_mClassLabel[j])
			{
				CovMat.block(j*feat_dim,0,feat_dim,feat_dim) += (featmat.row(i)-MeanMat.row(j)).transpose()*(featmat.row(i)-MeanMat.row(j));
			}
		}
	}
	for(int i=0;i<cnum;i++)
	{
		PoolCovMat+=CovMat.block(i*feat_dim,0,feat_dim,feat_dim);
		CovMat.block(i*feat_dim,0,feat_dim,feat_dim)=CovMat.block(i*feat_dim,0,feat_dim,feat_dim)/(feat_num_perclass(i)-1);
	}
	PoolCovMat/=(feat_num-cnum);
	PoolCovMat = PoolCovMat.inverse();

	//transform the data format from Eigen to member vectors
	vector<double> temp;
	for(int i=0;i<cnum;i++)
	{
		for(int j=0;j<feat_dim;j++)
		{
			temp.push_back(MeanMat(i,j));
		}
		_mModelMean.push_back(temp);
		temp.clear();
	}
	for(int i=0;i<feat_dim;i++)
	{
		for(int j=0;j<feat_dim;j++)
		{
			temp.push_back(PoolCovMat(i,j));
		}
		_mModelCov.push_back(temp);
		temp.clear();
	}

	return true;
}

int LDA_Bayesian::Bayespredict(vector<vector<double> >& PoolCovMat, vector<vector<double> >& MeanMat, vector<double>& x)
{
	using namespace Eigen; 

	int feat_dim = PoolCovMat.size();
	int cnum = MeanMat.size();
	int dim_x = x.size();
	if(dim_x!=feat_dim)
	{
		std::cerr<<"the dimension of features is not equal to the dimension of Gaussian Model"<<std::endl;
		return -1;
	}
	MatrixXd feature = MatrixXd::Zero(1,feat_dim);
	MatrixXd Cov = MatrixXd::Zero(feat_dim,feat_dim);
	MatrixXd Mean = MatrixXd::Zero(cnum,feat_dim);
	//MatrixXd Imat = MatrixXd::Identity(feat_dim,feat_dim);
	int i,j;
	for(i=0;i<feat_dim;i++)
	{
		feature(0,i)=x[i];
	}
	for(i=0;i<feat_dim;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			Cov(i,j)=PoolCovMat[i][j];
		}
	}
	for(i=0;i<cnum;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			Mean(i,j)=MeanMat[i][j];
		}
	}
	double gx;
	double mingx;
	int predict;
	for(i=0;i<cnum;i++)
	{
		// 马氏距离，Cov是真正协方差的逆矩阵
		gx = (feature.row(0)-Mean.row(i))*Cov*(feature.row(0)-Mean.row(i)).transpose();
		if(i==0)
		{
			mingx = gx;
			predict = _mClassLabel[i];
		}
		else
		{
			if(gx<mingx)
			{
				mingx=gx;
				predict = _mClassLabel[i];
			}
		}
	}

	return predict;
}

void LDA_Bayesian::FeatureExtract(Vector2D& DataMatrix, Vector1I& Label)
{
	_mFeatureMatrix.clear();
	_mLabelVector.clear();
	// 一次性对所有数据进行特征采集
	for (size_t smp_idx=0; smp_idx<DataMatrix.size(); smp_idx += _mFeaWinWidth)
	{
		vector<double> fea_temp;
		int label_temp;
		fea_temp.resize(_mFeaDimension);
		int channel_index;

		// feature TD
		// 4 features: fea0-绝对值之和, fea1-过零点数, fea2-波长, fea3-导数拐点
		for(int i=0;i<_mChannelNum;i++)//LB// _mChannelNum=8 here
		{
			channel_index = i;
			fea_temp[i*4]=0;fea_temp[i*4+1]=0;fea_temp[i*4+2]=0;fea_temp[i*4+3]=0;			
			for(int j=0;j<_mFeaWinWidth;j++)
			{
				// _mFeaWinWidth数据窗中的数据，j0是当前采样，j1是上一次采样，j2是再前一次采样;
				int j0 = smp_idx+j;
				int j1 = j0-1;
				int j2 = j0-2;

				//fea0// fea_temp[0 4 8……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据绝对值的和
				fea_temp[i*4]+=abs(DataMatrix[j0][channel_index]);
				if(j>0)
				{
					//fea1// fea_temp[1 5 9……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据相邻两个相乘是否大于0的状态的总和
					fea_temp[i*4+1]+=int(DataMatrix[j0][channel_index]*DataMatrix[j1][channel_index]>0);
					//fea2// fea_temp[2 6 10……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据相邻两个之差的绝对值的总和
					fea_temp[i*4+2]+=abs(DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index]);
				}
				if(j>1)
				{
					//fea3// fea_temp[3 8 11……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据相邻两个数据之差的乘积是否大于0的状态的总和
					fea_temp[i*4+3]+=int((DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index])*(DataMatrix[j1][channel_index]-DataMatrix[j2][channel_index])>0);
				}

				// label
				// take mid-window label as the window's label
				if (j==_mFeaWinWidth/2)
				{
					label_temp = Label[j0];
				}
			}
		}
				
		_mFeatureMatrix.push_back(fea_temp);
		_mLabelVector.push_back(label_temp);
	}
}

std::vector<double> LDA_Bayesian::FeatureExtractToVec( Vector2D& DataMatrix )
{
	vector<double> fea_temp;
	for (size_t smp_idx=0; smp_idx<DataMatrix.size(); smp_idx += _mFeaWinWidth)
	{
		fea_temp.resize(_mFeaDimension);
		int channel_index;

		// feature TD
		// 3 features: fea0-绝对值之和, fea1-过零点数, fea2-波长, fea3-导数拐点
		for(int i=0;i<_mChannelNum;i++) // _mChannelNum=8 here
		{
			channel_index = i;
			fea_temp[i*4]=0;fea_temp[i*4+1]=0;fea_temp[i*4+2]=0;fea_temp[i*4+3]=0;			
			for(int j=0;j<_mFeaWinWidth;j++)
			{
				// _mFeaWinWidth数据窗中的数据，j0是当前采样，j1是上一次采样，j2是再前一次采样;
				int j0 = smp_idx+j;
				int j1 = j0-1;
				int j2 = j0-2;

				//fea0// fea_temp[0 4 8……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据绝对值的和
				fea_temp[i*4]+=abs(DataMatrix[j0][channel_index]);
				if(j>0)
				{
					//fea1// fea_temp[1 5 9……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据相邻两个相乘是否大于0的状态的总和
					fea_temp[i*4+1]+=int(DataMatrix[j0][channel_index]*DataMatrix[j1][channel_index]>0);
					//fea2// fea_temp[2 6 10……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据相邻两个之差的绝对值的总和
					fea_temp[i*4+2]+=abs(DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index]);
				}
				if(j>1)
				{
					//fea3// fea_temp[3 8 11……]位上分别是0 1 2通道上前(_mFeaWinWidth)个数据相邻两个数据之差的乘积是否大于0的状态的总和
					fea_temp[i*4+3]+=int((DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index])*(DataMatrix[j1][channel_index]-DataMatrix[j2][channel_index])>0);
				}
			}
		}
	}
	return fea_temp;
}

bool LDA_Bayesian::GenerateModel()
{
	return Bayestrain(_mFeatureMatrix, _mLabelVector);
}

vector<int> LDA_Bayesian::GetClassVector()
{
	return _mClassLabel;
}

int LDA_Bayesian::Predict( vector<double>& x )
{
	return Bayespredict(_mModelCov, _mModelMean, x);
}

bool LDA_Bayesian::SaveModel( std::string& Dir_Name )
{
	//save the baseline model data
	std::ofstream savefile;
	savefile.open(Dir_Name+"\\LDA_mean.txt");
	if (!savefile.is_open())
	{
		std::cout << "Can't open file: LDA_mean.txt" << std::endl;
		return false;
	}

	// save mean matrix
	for(int i=0;i<_mModelMean.size();i++)
	{
		for(int j=0; j<_mModelMean[i].size(); j++)
			savefile<<_mModelMean[i][j]<<'\t';
		if(i != _mModelMean.size()-1)
			savefile<<'\n';
	}
	savefile.close();

	// save covariance matrix
	savefile.open(Dir_Name+"\\LDA_cov.txt");
	if (!savefile.is_open())
	{
		std::cout << "Can't open file: LDA_cov.txt" << std::endl;
		return false;
	}
	for(int i=0; i<_mModelCov.size(); i++)
	{
		for(int j=0; j<_mModelCov[i].size(); j++)
			savefile<<_mModelCov[i][j]<<'\t';
		if(i != _mModelCov.size()-1)
			savefile<<'\n';
	}
	savefile.close();

	return true;
}


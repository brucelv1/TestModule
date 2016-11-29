#include "LDA_Bayesian.h"
#include <iostream>
#include <fstream>
#include "eigen-3.2.1/Eigen/Dense"

LDA_Bayesian::LDA_Bayesian()
{
	_mFeaWinWidth = 200;
	_mFeaDimension = 24;
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

	//transform the data format from Eigen to Vector
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

	//save the baseline model data
	std::ofstream savefile;
	savefile.open(".\\data\\model\\LDA_mean.txt");
	if (!savefile.is_open())
	{
		std::cout << "Can't open file: .\\data\\model\\LDA_mean.txt" << std::endl;
		return false;
	}
	for(int i=0;i<cnum;i++)
	{
		for(int j=0;j<feat_dim;j++)
			savefile<<_mModelMean[i][j]<<'\t';
		savefile<<'\n';
	}
	savefile.close();


	savefile.open(".\\data\\model\\LDA_cov.txt");
	if (!savefile.is_open())
	{
		std::cout << "Can't open file: .\\data\\model\\LDA_cov.txt" << std::endl;
		return false;
	}
	for(int i=0;i<feat_dim;i++)
	{
		for(int j=0;j<feat_dim;j++)
			savefile<<_mModelCov[i][j]<<'\t';
		savefile<<'\n';
	}
	savefile.close();

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
		// ���Ͼ��룬Cov������Э����������
		gx = (feature.row(0)-Mean.row(i))*Cov*(feature.row(0)-Mean.row(i)).transpose();
		if(i==0)
		{
			mingx = gx;
			predict = i;
		}
		else
		{
			if(gx<mingx)
			{
				mingx=gx;
				predict = i;
			}
		}
	}

	return predict;
}

void LDA_Bayesian::FeatureExtract(Vector2D& DataMatrix, Vector1I& Label)
{
	_mFeatureMatrix.clear();
	_mLabelVector.clear();
	// һ���Զ��������ݽ��������ɼ�
	for (size_t smp_idx=0; smp_idx<DataMatrix.size(); smp_idx += _mFeaWinWidth)
	{
		vector<double> fea_temp;
		int label_temp;
		fea_temp.resize(_mFeaDimension);
		int channel_index;


		// feature TD
		// 3 features: fea0-����ֵ֮��, fea1-�������, fea2-����, fea3-�����յ�
		for(int i=0;i<4;i++)//LB// _mChannelNum=8 here
		{
			channel_index = i; // 1 2 3 4
			fea_temp[i*4]=0;fea_temp[i*4+1]=0;fea_temp[i*4+2]=0;fea_temp[i*4+3]=0;			
			for(int j=0;j<_mFeaWinWidth;j++)
			{
				// _mFeaWinWidth���ݴ��е����ݣ�j0�ǵ�ǰ������j1����һ�β�����j2����ǰһ�β���;
				int j0 = smp_idx+j;
				int j1 = j0-1;
				int j2 = j0-2;

				//fea0// fea_temp[0 4 8����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)�����ݾ���ֵ�ĺ�
				fea_temp[i*4]+=abs(DataMatrix[j0][channel_index]);
				if(j>0)
				{
					//fea1// fea_temp[1 5 9����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)������������������Ƿ����0��״̬���ܺ�
					fea_temp[i*4+1]+=int(DataMatrix[j0][channel_index]*DataMatrix[j1][channel_index]>0);
					//fea2// fea_temp[2 6 10����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)��������������֮��ľ���ֵ���ܺ�
					fea_temp[i*4+2]+=abs(DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index]);
				}
				if(j>1)
				{
					//fea3// fea_temp[3 8 11����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)������������������֮��ĳ˻��Ƿ����0��״̬���ܺ�
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

		// feature NIR -- I do not care
		for(int i=4;i<_mChannelNum;i++)//LB// _mChannelNum=8 here
		{
			channel_index = i; // 5 6 7 8
			fea_temp[4*4+(i-4)*2]=0;fea_temp[4*4+(i-4)*2+1]=0;			
			for(int j=0;j<_mFeaWinWidth;j++)
			{
				// _mFeaWinWidth���ݴ��е����ݣ�j0�ǵ�ǰ������j1����һ�β���;
				int j0 = smp_idx+j;
				int j1 = j0-1;

				//fea0// fea_temp[16 18 20 22]λ�Ϸֱ���5 6 7 8ͨ����ǰ(_mFeaWinWidth)�����ݾ���ֵ�ĺ�
				fea_temp[4*4+(i-4)*2]+=DataMatrix[j0][channel_index];   
				if(j>0)
				{
					//fea2// fea_temp[17 19 21 23]λ�Ϸֱ���5 6 7 8ͨ����ǰ(_mFeaWinWidth)��������������֮��ľ���ֵ���ܺ�
					fea_temp[4*4+(i-4)*2+1]+=abs(DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index]);
				}

			}
			fea_temp[4*4+(i-4)*2]=fea_temp[4*4+(i-4)*2]/_mFeaWinWidth;
		}
		
		if(1)
		{
			_mFeatureMatrix.push_back(fea_temp);
			_mLabelVector.push_back(label_temp);
		}
		//else
		//{
		//	//LB// ����ʱ����SVM�����жϽ��
		//	if(0)
		//	{
		//		//����SVM�����жϽ��
		//		for(int k=0;k<_mFeaDimension;k++)
		//		{
		//			x[k].index=k;
		//			x[k].value=fea_temp[k];
		//		}
		//		data_predict[mat_index%DATA_LENGTH]=int(svm_predict(model,x));
		//	}
		//	else
		//	{
		//		// Byespredict is called only here
		//		data_predict[mat_index%DATA_LENGTH]=int(Byespredict(lda_model_cov,lda_model_mean,fea_temp));
		//	}
		//	//LB// ����double data_predict[DATA_LENGTH]��ÿ�����ݵ�ģʽ״̬

		//	if(IS_OnLineTest)
		//	{
		//		random_figure=class_num-1;//LB//һ��״̬��random_figureΪREST

		//		if((mat_index/1000)%ACTION_TIME_ONLINE_TESTING>=REST_TIME_ONLINE_TESTING)//LB//���߲���״̬�£�ǰ3sΪREST����6sΪ�������
		//		{
		//			random_figure=random_hand_serial[mat_index/(1000*ACTION_TIME_ONLINE_TESTING)];

		//			if(data_predict[mat_index%DATA_LENGTH]==random_figure)
		//				Is_Correct_Num++;
		//		}
		//		else
		//		{
		//			Is_Correct_Num = 0;
		//			Is_Action = false;
		//		}
		//		online_set_label.push_back(random_figure);
		//		online_label.push_back(data_predict[mat_index%DATA_LENGTH]);//LB// ��data_predict����label��β��   Ϊʲô��50���Ļ�
		//	}
		//}
	}
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

std::vector<double> LDA_Bayesian::FeatureExtractToVec( Vector2D& DataMatrix )
{
	vector<double> fea_temp;
	for (size_t smp_idx=0; smp_idx<DataMatrix.size(); smp_idx += _mFeaWinWidth)
	{
		fea_temp.resize(_mFeaDimension);
		int channel_index;

		// feature TD
		// 3 features: fea0-����ֵ֮��, fea1-�������, fea2-����, fea3-�����յ�
		for(int i=0;i<4;i++)//LB// _mChannelNum=8 here
		{
			channel_index = i; // 1 2 3 4
			fea_temp[i*4]=0;fea_temp[i*4+1]=0;fea_temp[i*4+2]=0;fea_temp[i*4+3]=0;			
			for(int j=0;j<_mFeaWinWidth;j++)
			{
				// _mFeaWinWidth���ݴ��е����ݣ�j0�ǵ�ǰ������j1����һ�β�����j2����ǰһ�β���;
				int j0 = smp_idx+j;
				int j1 = j0-1;
				int j2 = j0-2;

				//fea0// fea_temp[0 4 8����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)�����ݾ���ֵ�ĺ�
				fea_temp[i*4]+=abs(DataMatrix[j0][channel_index]);
				if(j>0)
				{
					//fea1// fea_temp[1 5 9����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)������������������Ƿ����0��״̬���ܺ�
					fea_temp[i*4+1]+=int(DataMatrix[j0][channel_index]*DataMatrix[j1][channel_index]>0);
					//fea2// fea_temp[2 6 10����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)��������������֮��ľ���ֵ���ܺ�
					fea_temp[i*4+2]+=abs(DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index]);
				}
				if(j>1)
				{
					//fea3// fea_temp[3 8 11����]λ�Ϸֱ���0 1 2ͨ����ǰ(_mFeaWinWidth)������������������֮��ĳ˻��Ƿ����0��״̬���ܺ�
					fea_temp[i*4+3]+=int((DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index])*(DataMatrix[j1][channel_index]-DataMatrix[j2][channel_index])>0);
				}
			}
		}

		// feature NIR -- I do not care
		for(int i=4;i<_mChannelNum;i++)//LB// _mChannelNum=8 here
		{
			channel_index = i; // 5 6 7 8
			fea_temp[4*4+(i-4)*2]=0;fea_temp[4*4+(i-4)*2+1]=0;			
			for(int j=0;j<_mFeaWinWidth;j++)
			{
				// _mFeaWinWidth���ݴ��е����ݣ�j0�ǵ�ǰ������j1����һ�β���;
				int j0 = smp_idx+j;
				int j1 = j0-1;

				//fea0// fea_temp[16 18 20 22]λ�Ϸֱ���5 6 7 8ͨ����ǰ(_mFeaWinWidth)�����ݾ���ֵ�ĺ�
				fea_temp[4*4+(i-4)*2]+=DataMatrix[j0][channel_index];   
				if(j>0)
				{
					//fea2// fea_temp[17 19 21 23]λ�Ϸֱ���5 6 7 8ͨ����ǰ(_mFeaWinWidth)��������������֮��ľ���ֵ���ܺ�
					fea_temp[4*4+(i-4)*2+1]+=abs(DataMatrix[j0][channel_index]-DataMatrix[j1][channel_index]);
				}

			}
			fea_temp[4*4+(i-4)*2]=fea_temp[4*4+(i-4)*2]/_mFeaWinWidth;
		}	
	}
	return fea_temp;
}

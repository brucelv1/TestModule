#include "Byesclassifier.h"

void Byestrain(vector<vector<double> > feat, vector<int> label)
{
	//number of features and dimension of features
	//number of labels
	int i,j;
	int feat_num = feat.size();
	int feat_dim = feat[0].size();
	int lab_num = label.size();
	if(lab_num!=feat_num)
	{
		std::cerr<<"the number of features is not equal to the number of labels"<<std::endl;
		getchar();
	}
	//unique label of labels
	vector<int> unilabel;
	for(i=0;i<lab_num;i++)
		unilabel.push_back(label[i]);

	sort(unilabel.begin(), unilabel.end());
	unilabel.erase(unique(unilabel.begin(), unilabel.end()), unilabel.end());
	//Eigen matrix class
	MatrixXd featmat(feat_num,feat_dim);
	for(i=0;i<feat_num;i++)
	{
		for (j=0;j<feat_dim;j++)
		{
			featmat(i,j)=feat[i][j];
		}
	}
	//number of classes
	int cnum = unilabel.size();
	// mean matrix and covariance matrix for each class
	MatrixXd MeanMat = MatrixXd::Zero(cnum,feat_dim);
	MatrixXd CovMat = MatrixXd::Zero(feat_dim*cnum,feat_dim);
	// pool covariance matrix
	MatrixXd PoolCovMat = MatrixXd::Zero(feat_dim,feat_dim);
	// sample number for each class
	VectorXd feat_num_perclass = VectorXd::Zero(cnum);
	// compute the mean vector for each class
	for(i=0;i<feat_num;i++)
	{
		for(j=0;j<cnum;j++)
		{
			if(label[i]==unilabel[j])
			{
				MeanMat.row(j)=MeanMat.row(j)+ featmat.row(i);
				feat_num_perclass(j) = feat_num_perclass(j)+1;
			}
		}
	}
	for(i=0;i<cnum;i++)
	{
		MeanMat.row(i)=MeanMat.row(i)/feat_num_perclass(i);
	}
	//compute the covariance matrix for each class and pool covariance matrix
	for(i=0;i<feat_num;i++)
	{
		for(j=0;j<cnum;j++)
		{
			if(label[i]==unilabel[j])
			{
				CovMat.block(j*feat_dim,0,feat_dim,feat_dim) += (featmat.row(i)-MeanMat.row(j)).transpose()*(featmat.row(i)-MeanMat.row(j));
			}
		}
	}
	for(i=0;i<cnum;i++)
	{
		PoolCovMat+=CovMat.block(i*feat_dim,0,feat_dim,feat_dim);
		CovMat.block(i*feat_dim,0,feat_dim,feat_dim)=CovMat.block(i*feat_dim,0,feat_dim,feat_dim)/(feat_num_perclass(i)-1);
	}
	PoolCovMat/=(feat_num-cnum);
	PoolCovMat = PoolCovMat.inverse();

	//transform the data formate from Eigen to Vector
	vector<vector<double> > modelmean;
	vector<vector<double> > modelcov;
	vector<double> temp;
	for(i=0;i<cnum;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			temp.push_back(MeanMat(i,j));
		}
		modelmean.push_back(temp);
		temp.clear();
	}
	for(i=0;i<feat_dim;i++)
	{
		for(j=0;j<feat_dim;j++)
		{
			temp.push_back(PoolCovMat(i,j));
		}
		modelcov.push_back(temp);
		temp.clear();
	}

	//save the baseline model data
	std::ofstream savefile;
	savefile.open(".\\data\\LDA_mean.txt");
	for(i=0;i<cnum;i++)
	{
		for(j=0;j<feat_dim;j++)
			savefile<<modelmean[i][j]<<'\t';
		savefile<<'\n';
	}
	savefile.close();

	savefile.open(".\\data\\LDA_cov.txt");
	for(i=0;i<feat_dim;i++)
	{
		for(j=0;j<feat_dim;j++)
			savefile<<modelcov[i][j]<<'\t';
		savefile<<'\n';
	}
	savefile.close();
}

int Byespredict(vector<vector<double> > PoolCovMat,vector<vector<double> > MeanMat,vector<double> x)
{
	int feat_dim = PoolCovMat.size();
	int cnum = MeanMat.size();
	int dim_x = x.size();
	if(dim_x!=feat_dim)
	{
		std::cerr<<"the dimension of features is not equal to the dimension of Gaussian Model"<<std::endl;
		getchar();
	}
	MatrixXd feat = MatrixXd::Zero(1,feat_dim);
	MatrixXd Cov = MatrixXd::Zero(feat_dim,feat_dim);
	MatrixXd Mean = MatrixXd::Zero(cnum,feat_dim);
	//MatrixXd Imat = MatrixXd::Identity(feat_dim,feat_dim);
	int i,j;
	for(i=0;i<feat_dim;i++)
	{
		feat(0,i)=x[i];
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
		gx = (feat.row(0)-Mean.row(i))*Cov*(feat.row(0)-Mean.row(i)).transpose();
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
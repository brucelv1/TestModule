#pragma once

#include <vector>
using std::vector;

class LDA_Bayesian
{
private:
	typedef std::vector<int> Vector1I;
	typedef std::vector<std::vector<double> > Vector2D;

	// pre-model data, used to generate model parameters
	Vector2D _mFeatureMatrix;
	Vector1I _mLabelVector;

	// unique label: it means different classes
	vector<int> _mClassLabel;

	// model parameters
	Vector2D _mModelMean;
	Vector2D _mModelCov;
public:
	// window width of feature extraction
	int _mFeaWinWidth;
private:
	// feature dimension
	int _mFeaDimension;
	// number of data channels
	int _mChannelNum;

public:
	LDA_Bayesian();
	~LDA_Bayesian();
	
	// extract feature from source data and label, used to generate model
	void FeatureExtract(Vector2D& DataMatrix, Vector1I& Label);

	// extract feature from source data, used to do prediction
	std::vector<double> FeatureExtractToVec(Vector2D& DataMatrix);

	// generate model parameters and save, using extracted feature and label
	bool GenerateModel();

	// unique label: it means different classes
	vector<int> GetClassVector();

	// take a vector, and predict an int
	int Predict(vector<double>& x);

private:
	// take feature and label, calculate mean and covariance of LDA model, and finally save
	// parameters to disk, at ./data/model/
	bool Bayestrain(vector<vector<double> >& feature, vector<int>& label);

	// take PoolCovMat, MeanMat and feature of sample (x), calculate label
	int Bayespredict(vector<vector<double> >& PoolCovMat, vector<vector<double> >& MeanMat, vector<double>& x);
};
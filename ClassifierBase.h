#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;
class ClassifierBase
{
public:
	ClassifierBase() {}
	virtual ~ClassifierBase() {}
	// name of this classifier
	virtual string GetName()=0;
	// sampling window length
	virtual int GetWindowLength()=0;
	// add new feature vectors and label vectors
	virtual int AddFeatureLabelFromData(vector<vector<double>>& data, vector<int>& label, int* totalFea, int* totalLabel)=0;
	// calculate model parameters
	virtual bool GenerateModel()=0;
	// take a slice of data (DataWindow) and predict
	virtual int Predict(vector<vector<double>>& dataWindow)=0;
	// save anything you want onto disk, basically this is used to save model parameters
	virtual bool SaveModelToDisk(string& directory)=0;	
	//unique label: it means different classes
	virtual vector<int> GetClassVector() = 0;
};

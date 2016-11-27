#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "eigen-3.2.1/Eigen/Dense"
#include "Config.h"

using std::vector;
using namespace Eigen;  
using namespace Eigen::internal;  
using namespace Eigen::Architecture;

#ifdef __cplusplus
extern "C" {
#endif

void Byestrain(vector<vector<double> > feat, vector<int> label);

int Byespredict(vector<vector<double> > PoolCovMat,vector<vector<double> > MeanMat,vector<double> x);

#ifdef __cplusplus
}
#endif
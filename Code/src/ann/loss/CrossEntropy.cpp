#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction): ILossLayer(reduction){
    
}

CrossEntropy::CrossEntropy(const CrossEntropy& orig):
ILossLayer(orig){
}

CrossEntropy::~CrossEntropy() {
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t){
    this->m_aYtarget = t;
    this->m_aCached_Ypred = X;  
    const double EPSILON = 1e-7; 
    auto x = xt::clip(X, EPSILON, 1.0 - EPSILON);  
    
    auto loss = -xt::sum(t * xt::log(x)) / X.shape()[0];
    return loss();
}
xt::xarray<double> CrossEntropy::backward() {
    const double EPSILON = 1e-7;
  
    auto y = xt::clip(this->m_aCached_Ypred, EPSILON, 1.0 - EPSILON);
    
    xt::xarray<double> dx = -(this->m_aYtarget / y) / y.shape()[0];  
    
    return dx;
}
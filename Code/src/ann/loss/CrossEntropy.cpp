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
    return cross_entropy(X, t, REDUCE_MEAN);
}
xt::xarray<double> CrossEntropy::backward() {
    const double EPSILON = 1e-7;
    xt::xarray<double> dx = -(m_aYtarget / (m_aCached_Ypred + EPSILON));
    dx /= m_aCached_Ypred.shape(0);
    return dx;
}

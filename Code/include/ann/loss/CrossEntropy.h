#ifndef CROSSENTROPY_H
#define CROSSENTROPY_H
#include "loss/ILossLayer.h"

class CrossEntropy: public ILossLayer {
public:
    //CrossEntropy(int nclasses=2, LossReduction reduction=REDUCE_MEAN);
    CrossEntropy(LossReduction reduction=REDUCE_MEAN);
    CrossEntropy(const CrossEntropy& orig);
    virtual ~CrossEntropy();
    
    virtual double forward(xt::xarray<double> X, xt::xarray<double> t);
    virtual xt::xarray<double> backward();
    
private:
    xt::xarray<double> m_aYtarget;
    xt::xarray<double> m_aCached_Ypred;  
    //int m_nClasses;
};

#endif 


#ifndef LOSSLAYER_H
#define LOSSLAYER_H
#include "tensor/xtensor_lib.h"

enum LossReduction{
    REDUCE_NONE = 0,
    REDUCE_SUM,
    REDUCE_MEAN
};

class ILossLayer {
public:
    ILossLayer(LossReduction reduction=REDUCE_MEAN);
    ILossLayer(const ILossLayer& orig);
    virtual ~ILossLayer();
    
    virtual double forward(xt::xarray<double> X, xt::xarray<double> t)=0;
    virtual xt::xarray<double> backward()=0;
private:
    LossReduction m_eReduction;
};

#endif 


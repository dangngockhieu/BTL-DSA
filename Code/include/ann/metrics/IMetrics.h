#ifndef METRICLAYER_H
#define METRICLAYER_H

#include "tensor/xtensor_lib.h"

class IMetrics {
public:
    IMetrics(int nOutputs);
    IMetrics(const IMetrics& orig);
    virtual ~IMetrics();
    virtual double evaluate(xt::xarray<double> pred, xt::xarray<double> target);
    //
    virtual void reset_metrics()=0;
    virtual void accumulate(double_tensor y_true, double_tensor y_pred);
    virtual double_tensor calculate_metrics(double_tensor y_true, double_tensor y_pred) = 0;
    virtual const double_tensor& get_metrics(){return m_metrics; };
    virtual ulong get_counts(){ return m_sample_counter; }
    
protected:
    ulong m_sample_counter;
    double_tensor m_metrics;
    int m_nOutputs;
    
};

#endif 


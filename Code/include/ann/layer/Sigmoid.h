#ifndef SIGMOID_H
#define SIGMOID_H
#include "layer/ILayer.h"

class Sigmoid: public ILayer {
public:
    Sigmoid(string name="");
    Sigmoid(const Sigmoid& orig);
    virtual ~Sigmoid();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    xt::xarray<double> backward(xt::xarray<double> DY);
    
    string get_desc();
    LayerType get_type(){ return LayerType::SIGMOID; };
private:
    xt::xarray<double> m_aCached_Y;

};

#endif 


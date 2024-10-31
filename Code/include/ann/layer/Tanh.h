#ifndef TANH_H
#define TANH_H
#include "layer/ILayer.h"

class Tanh: public ILayer  {
public:
    Tanh(string name="");
    Tanh(const Tanh& orig);
    virtual ~Tanh();
    
    xt::xarray<double> forward(xt::xarray<double> X);
    xt::xarray<double> backward(xt::xarray<double> DY);
    
    string get_desc();
    LayerType get_type(){ return LayerType::TANH; };
private:
    xt::xarray<double> m_aCached_Y;
};

#endif 


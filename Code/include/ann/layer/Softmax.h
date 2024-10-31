#ifndef SOFTMAX_H
#define SOFTMAX_H
#include "layer/ILayer.h"

class Softmax: public ILayer {
public:
    Softmax(int axis=-1, string name="");
    Softmax(const Softmax& orig);
    virtual ~Softmax();

    virtual xt::xarray<double> forward(xt::xarray<double> X);
    virtual xt::xarray<double> backward(xt::xarray<double> DY);
    
    string get_desc();
    LayerType get_type(){ return LayerType::SOFTMAX; };
    
    //void save(string model_path);
    //void load(string model_path, string layer_name="");
private:
    int m_nAxis;
    xt::xarray<double> m_aCached_Y;    
};

#endif 


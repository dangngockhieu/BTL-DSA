#include "layer/Tanh.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

Tanh::Tanh(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::Tanh(const Tanh& orig) {
    m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::~Tanh() {
}

xt::xarray<double> Tanh::forward(xt::xarray<double> X) {
    this->m_aCached_Y = (xt::exp(X)-xt::exp(-X))/(xt::exp(X)+ xt::exp(-X));  
    return m_aCached_Y;
}
xt::xarray<double> Tanh::backward(xt::xarray<double> DY) {
     xt::xarray<double> x = 1 - this->m_aCached_Y * this->m_aCached_Y;
    
    return DY * x;
}

string Tanh::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Tanh", this->getname());
    return desc;
}

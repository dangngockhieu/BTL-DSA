#include "layer/ReLU.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

ReLU::ReLU(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "ReLU_" + to_string(++m_unLayer_idx);
}

ReLU::ReLU(const ReLU& orig) {
    m_sName = "ReLU_" + to_string(++m_unLayer_idx);
}

ReLU::~ReLU() {
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X) {
    auto shape = X.shape();
    xt::xarray<double> result = xt::zeros<double>(shape);
    
    this->m_aMask = xt::zeros<bool>(shape);
    for (size_t i = 0; i < X.size(); ++i) {
        if (X[i] <= 0) {
            result[i] = 0;
            m_aMask[i] = false;  
        } else {
            result[i] = X[i];
            m_aMask[i] = true;
        }
    }
    
    return result;
}
xt::xarray<double> ReLU::backward(xt::xarray<double> DY) {
    xt::xarray<double> a = xt::zeros<double>(DY.shape());
    
    for (size_t i = 0; i < DY.size(); ++i) {
        if (!(m_aMask[i])) {
            a[i] = 0; 
        } else {
            a[i] = DY[i]; 
        }
    }
    
    return a;
}

string ReLU::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "ReLU", this->getname());
    return desc;
}
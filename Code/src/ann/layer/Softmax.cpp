#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name): m_nAxis(axis) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    auto a = xt::exp(X); 
    auto sum_a = xt::sum(a, {m_nAxis}, xt::keep_dims);  
    
    this->m_aCached_Y = a / sum_a;
    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    auto denta_Y = this->m_aCached_Y * DY;  
    auto sum_denta_Y = xt::sum(denta_Y, {m_nAxis}, xt::keep_dims); 
    auto dX = denta_Y - (sum_denta_Y * m_aCached_Y); 
    return dX;
}

string Softmax::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                    "Softmax", this->getname(), m_nAxis);
    return desc;
}

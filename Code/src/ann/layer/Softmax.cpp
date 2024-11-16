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
    this->m_aCached_Y = softmax(X , this->m_nAxis);
    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
   // Check if DY has more than one dimension
    if (DY.dimension() != 1) {
        // Compute the outer product of m_aCached_Y with itself
        xt::xarray<double> b = outer_stack(m_aCached_Y, m_aCached_Y);
        // Create a diagonal matrix from m_aCached_Y
        xt::xarray<double> a = diag_stack(this->m_aCached_Y);
        // Subtract the outer product from the diagonal matrix
        xt::xarray<double> c = a - b;
        // Multiply the resulting matrix with DY
        xt::xarray<double> dX = matmul_on_stack(c, DY);
        return dX;
    }
    // Single dimension case
    // Create a diagonal matrix from m_aCached_Y
    xt::xarray<double> a = xt::diag(this->m_aCached_Y);
    // Subtract the outer product from the diagonal matrix
    xt::xarray<double> b = a - xt::linalg::outer(this->m_aCached_Y, this->m_aCached_Y);
    // Multiply the resulting matrix with DY
    xt::xarray<double> dX = xt::linalg::dot(b, DY);
    return dX;
}

string Softmax::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                    "Softmax", this->getname(), m_nAxis);
    return desc;
}

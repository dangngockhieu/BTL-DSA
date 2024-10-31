#ifndef IPARAMGROUP_H
#define IPARAMGROUP_H
#include "tensor/xtensor_lib.h"
#include "ann/functions.h"
#include <string>
using namespace std;
#include "dsaheader.h"

class IParamGroup {
public:
    IParamGroup(){};
    IParamGroup(const IParamGroup& orig){};
    virtual ~IParamGroup(){};
    virtual void register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad)=0;
    virtual void register_sample_count(unsigned long long* pCounter)=0;
    virtual void zero_grad()=0;
    virtual void step(double lr)=0;
private:

};

#endif


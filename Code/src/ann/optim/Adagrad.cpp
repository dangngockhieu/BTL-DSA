#include "optim/Adagrad.h"
#include "optim/AdaParamGroup.h"

Adagrad::Adagrad(double learning_rate, double decay): 
    IOptimizer(learning_rate), m_decay(decay){
}

Adagrad::Adagrad(const Adagrad& orig) {
}

Adagrad::~Adagrad() {
}

IParamGroup* Adagrad::create_group(string name){
    return new AdaParamGroup(m_decay);
}


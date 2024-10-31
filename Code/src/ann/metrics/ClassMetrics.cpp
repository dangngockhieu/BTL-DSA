#include "metrics/ClassMetrics.h"
#include "ann/functions.h"

ClassMetrics::ClassMetrics(int nClasses): IMetrics(nClasses) {
    m_sample_counter = 0;
    m_metrics = xt::zeros<double>({NUM_CLASS_METRICS});
}

ClassMetrics::ClassMetrics(const ClassMetrics& orig): IMetrics(orig.m_nOutputs)  {
    m_sample_counter = orig.m_sample_counter;
    m_metrics = orig.m_metrics;
}

ClassMetrics::~ClassMetrics() {
}

void ClassMetrics::reset_metrics(){  
    this->m_metrics = xt::zeros<double>({NUM_CLASS_METRICS});
    this->m_sample_counter = 0;
}
double_tensor ClassMetrics::calculate_metrics(double_tensor y_true, double_tensor y_pred){
    return calc_classifcation_metrics(y_true, y_pred, m_nOutputs);
}
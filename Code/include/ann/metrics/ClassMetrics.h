#ifndef CLASSMETRICS_H
#define CLASSMETRICS_H
#include "metrics/IMetrics.h"

class ClassMetrics: public IMetrics{
public:
    ClassMetrics(int nClasses);
    ClassMetrics(const ClassMetrics& orig);
    virtual ~ClassMetrics();
    
    void reset_metrics();
    double_tensor calculate_metrics(double_tensor y_true, double_tensor y_pred);
private:

};

#endif 


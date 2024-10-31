/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template <typename DType, typename LType> class DataLabel {
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

public:
  DataLabel(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  xt::xarray<DType> getData() const { return data; }
  xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType> class Batch {
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

public:
  Batch() : data(xt::xarray<DType>()), label(xt::xarray<LType>()) {}
  Batch(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  virtual ~Batch() {}
  xt::xarray<DType> &getData() { return data; }
  xt::xarray<LType> &getLabel() { return label; }
};

template <typename DType, typename LType> class Dataset {
private:
public:
  Dataset(){};
  virtual ~Dataset(){};

  virtual int len() = 0;
  virtual DataLabel<DType, LType> getitem(int index) = 0;
  virtual xt::svector<unsigned long> get_data_shape() = 0;
  virtual xt::svector<unsigned long> get_label_shape() = 0;
};
//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType> {
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;
  xt::svector<unsigned long> data_shape, label_shape;

public:
  TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {
    
    if (label.shape().size() == 0) {
        label_shape = {};
    }
    else{
        label_shape=label.shape();
    }
    data_shape=data.shape();
  }

  int len() { 
    if(this->data.dimension()!=0){
       return data_shape[0];
    }else{
      return 0; 
    }
  }

  DataLabel<DType, LType> getitem(int index) {
    if (index < 0 || index >= data.shape()[0]) {
      throw std::out_of_range("Index is out of range!");
    }
    xt::xarray<LType> s_label;

    if (label.dimension() > 0) {
        s_label = xt::view(label, index);
      }else {
        s_label = label;
      }
    xt::xarray<DType> s_data = xt::view(data, index);
    return DataLabel<DType, LType>{s_data, s_label};
  }

  xt::svector<unsigned long> get_data_shape() { return data_shape; }
  xt::svector<unsigned long> get_label_shape() { return label_shape; }
};

#endif /* DATASET_H */

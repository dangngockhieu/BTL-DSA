#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"
using namespace std;

template <typename DType, typename LType>
class DataLoader {
private:
    Dataset<DType, LType>* ptr_dataset; // Con trỏ tới dataset
    int batch_size;                     // Kích thước batch
    bool shuffle;                       // Xáo trộn dữ liệu
    bool drop_last;                     // Bỏ batch cuối nếu không đủ kích thước
    int num_batch;                      // Số lượng batch
    xt::xarray<int> indi;               // Danh sách chỉ số mẫu
    Batch<DType, LType>* batch;         // Mảng động lưu trữ các batch
    int m_seed;                         // Seed cho việc xáo trộn
   
public:
    // Constructor
    DataLoader(Dataset<DType, LType>* dataset, int batch_size, bool shuffle=true, bool drop_last=false, int seed=-1)
    : ptr_dataset(dataset), batch_size(batch_size), shuffle(shuffle), drop_last(drop_last), m_seed(seed) {

        int size = ptr_dataset->len();
        indi = xt::arange<int>(size);

        if (shuffle) {
            if (m_seed >= 0) {
                xt::random::seed(m_seed);
            }
            xt::random::shuffle(indi);
        }

        num_batch = size / batch_size;
   
        if(batch_size>size){
           num_batch=0;
        }
        batch = new Batch<DType, LType>[num_batch];

        int begin, stop;
        for (int t = 0; t < num_batch; ++t) {
            begin = t * batch_size; 
            if(begin + batch_size<=size){
                stop=begin + batch_size;
            }
            else{
                stop = size; 
            }

            if((!drop_last)&&t==num_batch-1){
                stop=size;
            }
            else if(drop_last&&t==num_batch-1){
                 stop=begin+batch_size;
            }
            int current_size = stop - begin;
            xt::xarray<LType>& c_label = batch[t].getLabel();
            xt::xarray<DType>& c_data = batch[t].getData();

            xt::svector<unsigned long> s_label_shape = ptr_dataset->get_label_shape();
            xt::svector<unsigned long> s_data_shape = ptr_dataset->get_data_shape();
            
            if(ptr_dataset->get_label_shape().size()!=0){
                 s_label_shape[0] = current_size;
            }
            
            if(ptr_dataset->get_label_shape().size()!=0){
                 c_label = xt::xarray<LType>::from_shape(s_label_shape);
            }
            s_data_shape[0] = current_size;
            c_data = xt::xarray<DType>::from_shape(s_data_shape);

            for (int i = begin; i < stop; i++) {
                if(ptr_dataset->get_label_shape().size()!=0){
                    xt::view(c_label, i - begin) = ptr_dataset->getitem(indi[i]).getLabel();
                }
                xt::view(c_data, i - begin) = ptr_dataset->getitem(indi[i]).getData();
            }
        }
}

    // Destructor
    virtual ~DataLoader() {
        delete[] batch;
    }
     
    int get_total_batch(){
        return this->num_batch;
    }

    Batch<DType, LType> get(int i){
        if(i >= num_batch){
            throw std::out_of_range("Index is out of range!");
        }
        return batch[i];
    }
 
    class Iterator {
    private:
        DataLoader<DType, LType>* loader;
        int c_batch;

    public:
        
        Iterator(DataLoader<DType, LType>* loader, int c_batch)
            : loader(loader), c_batch(c_batch) {}

        
        bool operator!=(const Iterator& tam) const {
            return c_batch != tam.c_batch;
        }
        
        Iterator& operator=(const Iterator& tam){
            if(this==&tam){
               return *this;
            }
            loader=tam.loader;
            c_batch=tam.c_batch;
            return *this;
        }
        
        Iterator& operator++() {
            ++c_batch;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Batch<DType, LType> operator*() {
            return loader->get(c_batch);
        }
    };

    Iterator begin(){
        return Iterator(this, 0);
    }

    Iterator end(){
        return Iterator(this, num_batch);
    }
};

#endif 

- Để phục vụ việc chấm bài, cần chỉnh sửa lại hàm khởi tạo của DataLoader bằng cách thêm thông số "seed" như sau:

+ Bản cũ:
DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false)
+ Bản mới:
DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false, int seed=-1)
Lưu ý: giá trị seed có thể âm hay dương.
Cách thực hiện: 
a. Lưu seed từ thông số vào một biến thành viên, gọi là m_seed.
b. Thêm vào đoạn mã để thực hiện:
   * Gọi hàm xt::random::seed(m_seed) NGAY TRƯỚC KHI gọi hàm xt::random::shuffle; với điều kiện là: shuffle=true và seed >= 0. 
    * Nghĩa là: 
          + Muốn shuffle các samples thì: truyền shuffle=true và seed <0; chạy các lần khác nhau, cho kết quả (thứ tự các samples) là khác nhau.
           + Nếu shuffle=true nhưng seed >=0: dữ liệu cũng shuffle, nhưng chạy nhiều lần vẫn cho cùng kết quả.

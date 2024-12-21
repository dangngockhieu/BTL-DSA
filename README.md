- Môn học Cấu trúc Dữ liệu và Giải thuật có 3 bài tập lớn. 
- Cả 3 bài đều theo một chủ đề chung: “Phát triển các cấu trúc dữ liệu và sử dụng chúng để xây dựng mạng nơron và đồ thị tính toán trong học sâu.” 
Các bài tập này có tính kế thừa lẫn nhau; cụ thể, Bài tập số 2 kế thừa từ Bài tập số 1, và Bài tập số 3 kế thừa từ cả hai bài trước đó. 
Do đó, cần chú ý hoàn thành các bài đầu tiên, đặc biệt là Bài tập số 1.

- Để phục vụ việc chấm bài, các bạn cần: chỉnh sửa lại hàm khởi tạo của DataLoader bằng cách thêm thông số "seed" như sau:

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

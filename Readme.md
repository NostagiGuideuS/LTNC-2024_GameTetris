 Bastard
<a name="ve-dau-trang"/>

##  Mục lục
* [1. Hướng dẫn cài đặt và chạy game](#cai-dat)
* [2. Mô tả chung về trò chơi](#mo-ta)
* [3. Các chức năng của trò chơi](#chuc-nang)
* [4. Các kĩ thuật lập trình được sử dụng](#ki-thuat)
* [5. Hỗ trợ](#ho-tro)
* [6. Kết luận](#ket-luan)
* [7. Tham Khảo](#tham-khao)

<a name="cai-dat"/>

## I, Hướng dẫn cài đặt và chạy game
  ### B1: Tải project về
Trên Github, chọn Code => Download ZIP  
  ### B2: Chạy game
  Tìm file Debug>TETRIS.exe và chạy file.

<a name="mo-ta"/>

## II, Mô tả chung về trò chơi
  **Thể loại:** `puzzle`
  
  Game TETRIS(xếp hình) cổ điển. Có một số khác biệt so với game gốc về cơ chế thay đổi các khối hình, dẫn tới một số kỹ thuật di chuyển có thể không thực hiện được.
  
<a name = "chuc-nang"/>

### III, Các chức năng của trò chơi
  - Điều khiển hình khối di chuyển bằng các phím `mũi tên` hoặc `ASD`
  - Thay đổi hình dạng bằng các phím `space` hoặc `Ctrl`
  - Đổi chỗ với hình khối ở hàng chờ bằng các phím `LShift` hoặc `Alt`
  - Không tồn tại trạng thái chiến thắng. Người chơi được ghi nhận thua cuộc khi các khối tồn dư cao tới đỉnh của màn chơi

<a name = "ki-thuat"/>

### IV, Các kỹ thuật lập trình được sử dụng
  - Sử dụng thư viện đồ họa SDL
  - Lập trình hướng đối tượng(OOP)
  - Con trỏ, cấp phát động
  - Sử dụng các thuật toán cơ bản như thuật toán xử lý va chạm, animation,....

<a name = "ho-tro"/>

### V, Hỗ trợ
- Sử dụng Photoshop để edit ảnh

<a name = "ket-luan"/>

### VI, Kết luận
  Fake n+1 :v
  

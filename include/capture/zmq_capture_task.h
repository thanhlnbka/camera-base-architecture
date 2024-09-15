// #include <zmq.hpp>
// #include <opencv2/opencv.hpp>
// #include <vector>
// #include <iostream>

// class ZMQCaptureTask : public CaptureTask {
// private:
//     std::string zmq_address;
//     zmq::context_t context;
//     zmq::socket_t socket;

// public:
//     ZMQCaptureTask(const std::string& address)
//         : zmq_address(address), context(1), socket(context, ZMQ_SUB) {
//         socket.connect(zmq_address);
//         socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);  // Subscribe tất cả message
//     }

//     // Triển khai phương thức cập nhật ảnh từ ZeroMQ
//     cv::Mat update_image() override {
//         zmq::message_t zmq_msg;
//         socket.recv(&zmq_msg);

//         // Chuyển đổi dữ liệu nhận từ ZMQ thành vector of uchar
//         std::vector<uchar> image_data((uchar*)zmq_msg.data(), (uchar*)zmq_msg.data() + zmq_msg.size());
//         return cv::imdecode(image_data, cv::IMREAD_COLOR);  // Trả về ảnh
//     }

//     ~ZMQCaptureTask() {
//         socket.close();
//     }
// };

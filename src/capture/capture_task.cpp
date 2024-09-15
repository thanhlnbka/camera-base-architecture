#include "capture/capture_task.h"

CaptureTask::CaptureTask() : running(false) {}

void CaptureTask::run() {
    running = true;
    capture_thread = std::thread([this]() {
        while (running) {
            cv::Mat new_frame = this->update_image();  
            if (!new_frame.empty()) {
                std::lock_guard<std::mutex> lock(frame_mutex);
                latest_frame = new_frame.clone();      
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(30));  
        }
    });
}
void CaptureTask::link_data(std::shared_ptr<DynamicProperties> obj, LimitedQueue<std::shared_ptr<DynamicProperties>>& queue) {
    running = true;
    link_data_thread = std::thread([this, obj, &queue]() {
        while (running) {
            cv::Mat latest_image = capture_image();
            if (!latest_image.empty()) {
                obj->set_property("image", latest_image); 
                auto obj_copy = obj->clone(); 
                if (capture_handler(obj_copy)) {
                    std::cout << "push obj to queue from capture handler !!" << std::endl;
                    queue.push(obj_copy);
                } else {
                    std::cout << "push obj default system !!" << std::endl;
                    queue.push(obj);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(30));  
        }
    });
}

void CaptureTask::stop() {
    running = false;
    if (capture_thread.joinable()) {
        capture_thread.join();  
    }

    if (link_data_thread.joinable()) {
        link_data_thread.join();
    }
}

cv::Mat CaptureTask::capture_image() {
    std::lock_guard<std::mutex> lock(frame_mutex);  
    return latest_frame.clone();  
}

CaptureTask::~CaptureTask() {
    stop();  
}

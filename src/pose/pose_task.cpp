#include "pose/pose_task.h"


PoseTask::PoseTask(const std::string& model_path): running(false) {
    
}


void PoseTask::run() {
    running = true;
}


void PoseTask::link_data(
    LimitedQueue<std::shared_ptr<DynamicProperties>>& input_queue,
    LimitedQueue<std::shared_ptr<DynamicProperties>>& output_queue) {
    running = true;
    link_data_thread = std::thread([this, &input_queue, &output_queue]() {
        while (running) {
            auto obj_ptr = input_queue.get();
            if (input_queue.get_size() > 0){
                auto image_opt = obj_ptr->get_property<cv::Mat>("image");
                if (image_opt) {
                    std::vector<int> pose = {640,640,640,640};
                    obj_ptr->set_property("pose", pose); 
                    auto obj_copy = obj_ptr->clone();
                    
                    if (pose_handler(obj_copy)) {
                        std::cout << "Push obj to queue from pose handler !!" << std::endl;
                        output_queue.push(obj_copy);
                    } else {
                        std::cout << "Push obj default system !!" << std::endl;
                        output_queue.push(obj_ptr);
                    }
                } else {
                    std::cout << "No image property found in the object." << std::endl;
                    
                }
            } 
            
            
            std::this_thread::sleep_for(std::chrono::milliseconds(30));  
        }
    });
}  


void PoseTask::stop() {
    running = false;
    if (link_data_thread.joinable()) {
        link_data_thread.join();
    }
}


PoseTask::~PoseTask() {
    stop();
}

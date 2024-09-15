#include "track/track_task.h"


TrackTask::TrackTask(const std::string& tracker_path): running(false) {
    
}


void TrackTask::run() {
    running = true;
}


void TrackTask::link_data(
    LimitedQueue<std::shared_ptr<DynamicProperties>>& input_queue,
    LimitedQueue<std::shared_ptr<DynamicProperties>>& output_queue) {
    running = true;
    link_data_thread = std::thread([this, &input_queue, &output_queue]() {
        while (running) {
            auto obj_ptr = input_queue.get();
            if (input_queue.get_size() > 0){
                auto pose = obj_ptr->get_property<std::vector<int>>("pose");
                if (pose) {
                    std::vector<int> track_ids = {1,2,3,4,5};
                    obj_ptr->set_property("track_ids", track_ids); 
                    auto obj_copy = obj_ptr->clone();
                    
                    if (track_handler(obj_copy)) {
                        std::cout << "Push obj to queue from track handler !!" << std::endl;
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


void TrackTask::stop() {
    running = false;
    if (link_data_thread.joinable()) {
        link_data_thread.join();
    }
}


TrackTask::~TrackTask() {
    stop();
}

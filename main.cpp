#include <iostream>
#include <thread>
#include <chrono>
#include "capture/rtsp_capture_task.h"
#include "pose/pose_task.h"
#include "track/track_task.h"
#include "handler/handler.h"
#include "utils/dynamic_properties.hpp"
#include "utils/limited_queue.hpp"

template <typename T>
void print_vector(std::string name_print, const std::vector<T>& vec) {
    std::cout << name_print <<  ": [ ";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "]" << std::endl;
}

bool capture_handler(std::shared_ptr<DynamicProperties> obj) {
    if (auto image_opt = obj->get_property<cv::Mat>("image")) {
        std::cout << "Captured image is available." << std::endl;
        cv::imwrite("image_capture_handler.jpg", *image_opt);
    } else {
        std::cout << "No image captured." << std::endl;
        return false;
    }
    return true;
}


bool pose_handler(std::shared_ptr<DynamicProperties> obj){
    if (auto pose = obj->get_property<std::vector<int>>("pose")) {
        auto image_opt = obj->get_property<cv::Mat>("image");
        cv::imwrite("image_pose_handler.jpg", *image_opt);
        std::cout << "Pose is available." << std::endl;
        print_vector("Pose", *pose);
        
    } else {
        std::cout << "No pose from image" << std::endl;
        return false;
    }
    return true;
}

bool track_handler(std::shared_ptr<DynamicProperties> obj){
    if (auto track_ids = obj->get_property<std::vector<int>>("track_ids")) {
        auto image_opt = obj->get_property<cv::Mat>("image");
        auto pose = obj->get_property<std::vector<int>>("pose");
        cv::imwrite("image_track_handler.jpg", *image_opt);
        std::cout << "Track is available." << std::endl;
        print_vector("Pose", *pose);
        print_vector("Track", *track_ids);
        
    } else {
        std::cout << "No track id from image" << std::endl;
        return false;
    }
    return true;
}


int main() {
    RTSPCaptureTask capture_task("rtsp://localhost:8554/test");
    PoseTask pose_task("path_model");
    TrackTask track_task("path_track");

    auto obj = std::make_shared<DynamicProperties>();
    LimitedQueue<std::shared_ptr<DynamicProperties>> capture_queue(100);
    LimitedQueue<std::shared_ptr<DynamicProperties>> pose_queue(100);
    LimitedQueue<std::shared_ptr<DynamicProperties>> track_queue(100);

    capture_task.link_data(obj, capture_queue);
    capture_task.run();

    pose_task.link_data(capture_queue, pose_queue);
    pose_task.run(); 

    track_task.link_data(pose_queue, track_queue);
    track_task.run();
    
    while (true) {
        std::cout << "CAPTURE QUEUE SIZE: " << capture_queue.get_size() << std::endl;
        std::cout << "POSE QUEUE SIZE: " << pose_queue.get_size() << std::endl;
        std::cout << "TRACK QUEUE SIZE: " << track_queue.get_size() << std::endl;
        auto track_obj_ptr = track_queue.get(); 
        if (track_obj_ptr) { 
            DynamicProperties& track_obj = *track_obj_ptr; 
            std::vector<std::string> list_properties = track_obj.list_properties();
            print_vector("Properties", list_properties);
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    return 0;
}

#ifndef POSE_TASK_H
#define POSE_TASK_H

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include "utils/limited_queue.hpp"
#include "utils/dynamic_properties.hpp"
#include <atomic>
#include <opencv2/opencv.hpp>
#include "handler/handler.h"

class PoseTask {
public:
    
    PoseTask(const std::string& model_path);

    
    ~PoseTask();

    
    void run();

    
    void link_data(LimitedQueue<std::shared_ptr<DynamicProperties>>& input_queue,
                      LimitedQueue<std::shared_ptr<DynamicProperties>>& output_queue);

    
    void stop();

private:
    
    std::string model_path;
    
    
    LimitedQueue<std::shared_ptr<DynamicProperties>>* input_queue;
    LimitedQueue<std::shared_ptr<DynamicProperties>>* output_queue;

    std::atomic<bool> running;     
    std::thread pose_thread;
    std::thread link_data_thread;
    
    cv::Mat process_pose(const cv::Mat& frame);

    
    std::mutex mutex;
};

#endif 

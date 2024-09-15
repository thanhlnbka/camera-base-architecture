#ifndef TRACK_TASK_H
#define TRACK_TASK_H

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include "utils/limited_queue.hpp"
#include "utils/dynamic_properties.hpp"
#include <opencv2/opencv.hpp>
#include "handler/handler.h"
#include <atomic>

class TrackTask {
public:
    
    TrackTask(const std::string& tracker_model_path);

    
    ~TrackTask();

    
    void run();

    
    void link_data(LimitedQueue<std::shared_ptr<DynamicProperties>>& input_queue,
                      LimitedQueue<std::shared_ptr<DynamicProperties>>& output_queue);

    
    void stop();

private:
    
    std::string tracker_model_path;

    
    LimitedQueue<std::shared_ptr<DynamicProperties>>* input_queue;
    LimitedQueue<std::shared_ptr<DynamicProperties>>* output_queue;

    
    std::atomic<bool> running;  
    std::thread link_data_thread;
    
    std::thread tracking_thread;

    
    cv::Mat process_tracking(const cv::Mat& frame);

    
    std::mutex mutex;
};

#endif 

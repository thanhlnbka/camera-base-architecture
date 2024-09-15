#ifndef CAPTURE_TASK_H
#define CAPTURE_TASK_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include "handler/handler.h"
#include "utils/dynamic_properties.hpp"
#include "utils/limited_queue.hpp"

class CaptureTask {
protected:
    cv::Mat latest_frame;       
    std::thread capture_thread; 
    std::thread link_data_thread;
    std::mutex frame_mutex;     
    std::atomic<bool> running;  

public:
    CaptureTask();

    
    virtual cv::Mat update_image() = 0;

    void link_data(std::shared_ptr<DynamicProperties> obj, LimitedQueue<std::shared_ptr<DynamicProperties>>& queue);

    
    void run();

    
    void stop();

    
    cv::Mat capture_image();

    
    virtual ~CaptureTask();
};

#endif 

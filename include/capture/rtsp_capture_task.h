#ifndef RTSP_CAPTURE_TASK_H
#define RTSP_CAPTURE_TASK_H

#include <opencv2/opencv.hpp>
#include <string>
#include "capture/capture_task.h"  



class RTSPCaptureTask : public CaptureTask {
private:
    std::string rtsp_url;      
    cv::VideoCapture cap;      

public:
    
    RTSPCaptureTask(const std::string& url);

    
    cv::Mat update_image() override;
};

#endif 

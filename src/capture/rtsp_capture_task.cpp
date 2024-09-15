#include "capture/rtsp_capture_task.h"
#include <iostream>


RTSPCaptureTask::RTSPCaptureTask(const std::string& url) : rtsp_url(url), cap(rtsp_url) {
    if (!cap.isOpened()) {
        std::cerr << "Cannot connect RTSP stream: " << rtsp_url << std::endl;
    }
}


cv::Mat RTSPCaptureTask::update_image() {
    cv::Mat frame;
    if (cap.isOpened()) {
        cap >> frame;  
        if (frame.empty()) {
            std::cerr << "Frame empty !" << std::endl;
        }
    }
    return frame;
}

#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

class APICaptureTask : public CaptureTask {
private:
    std::string api_url;

public:
    APICaptureTask(const std::string& url) : api_url(url) {}

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    
    cv::Mat update_image() override {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                std::cerr << "Lỗi gọi API: " << curl_easy_strerror(res) << std::endl;
                return cv::Mat();
            }

            std::vector<uchar> image_data(readBuffer.begin(), readBuffer.end());
            return cv::imdecode(image_data, cv::IMREAD_COLOR);  
        }

        return cv::Mat();  
    }
};

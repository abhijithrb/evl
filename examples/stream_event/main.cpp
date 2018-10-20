// Copyright 2018 Event Vision Library.
#include <iostream>
#include <thread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/types.hpp>
#include <evl/utils/event_utils.hpp>

void streamEvent(evl::EventBuffer *buffer, int lifetime) {
    usleep(100000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::vector<evl::EventTuple> v = evl::readBufferOnLifetime(buffer, lifetime);
        std::cout << "[Thread2] DATA READING =============" << std::endl;
        std::cout << "[Thread2] Lifetime >>> " << lifetime << std::endl;
        std::for_each((v).begin(), (v).end(), evl::printEvent);
        std::cout << "[Thread2] DATA READ DONE ===========" << std::endl;
    }
}

void streamEventAsImage(evl::EventBuffer *buffer, int lifetime) {
    cv::namedWindow("image", 1);
    usleep(100000);      // micro sec
    while (1) {
        usleep(100000);      // micro sec. calling frequency
        std::vector<evl::EventTuple> v = evl::readBufferOnLifetime(buffer, lifetime);
        cv::Mat img = evl::convertEventsToMat(v, true);
        cv::imshow("image", img);
        cv::waitKey(1);
    }
}

int main() {
    int lifetime = 5000;     // micro sec
    int buffersize = 50000;
    evl::EventBuffer buffer(buffersize);
    char fname[] = "../../data/sample2.csv";
    // std::thread t1(evl::storeBufferFromCsv, &buffer, fname);
    std::thread t1(evl::storeBufferFromDavis, &buffer);
    streamEvent(&buffer, lifetime);
    // streamEventAsImage(&buffer, lifetime);
    t1.join();
    return 0;
}

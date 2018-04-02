#ifndef __BASE64_H__
#define __BASE64_H__

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

std::string base64_decode(std::string const& encoded_string);
cv::Mat ImageDecode(std::string encode_data);
std::string ReadMessage(int soc);


#endif

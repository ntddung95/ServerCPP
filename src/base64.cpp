#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include "base64.h"

using namespace std;
using namespace cv;

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = 0; j < i; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}


string ReadMessage(int soc){
    bool running = true;
//    int bufLen;
    char cntLen[] = {0};
//    int test = 0;
    string dataFull;
    
    int numb = recv(soc, cntLen, 1000, 0);
    if(numb <= 0){
        exit(-1);
    }
//    bufLen = atoi(cntLen);
    while(running){
        char temp[1024];
        int total = 0;
        int n = recv(soc, &temp[total], 1024 - total - 1,0);
        if(n==-1){
           cout << "err" << endl;
           exit(-1);
        } else if (n == 0){
           running = false;
           break;
        }
        total += n;
        temp[total] ='\0';
        
        cout << "total: "  << total << endl;
        
        dataFull += temp;
        
//        test += total;
    }
//    cout << "test: " << test << endl;
//    cout << "bufLen: " << bufLen << endl;
    
    return dataFull;
}

cv::Mat ImageDecode(string encode_data){
    string decode_string = base64_decode(encode_data);
    //cout<<decode_string<<endl;
    //ofstream out("output1.txt");
    //out << decode_string;
    //out.close();
    vector<uchar> data(decode_string.begin(), decode_string.end());
    //Mat m(100,100,0,(void*)decode_string.data());
    cv::Mat m = cv::imdecode(data, cv::IMREAD_UNCHANGED);
    
    std::cout << "encode length: " <<encode_data.length() << std::endl;
    std::cout << "decode length: " << decode_string.length() << std::endl;
    std::cout << "img rows: " << m.rows << std::endl;
    std::cout << "img cols: " << m.cols << std::endl;
    
    return m;
}

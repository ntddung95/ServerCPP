#include "config.h"
#include "base64.h"
#include <iostream>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

using namespace cv;
using namespace std;
#define PORT 5000

int main( int argc, char **argv){
    struct sockaddr_in address;
    int server_fd, new_socket, valread;
    int opt = 1;
    struct sockaddr_in serv_addr;


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(true){

        if ((new_socket = accept(server_fd, NULL, NULL)))
        {   
            
            perror("accept");
            //exit(EXIT_FAILURE);
        }
        cout <<"XXX"<<endl;
        string dataImage1 = ReadMessage(new_socket);
        Mat img1 = ImageDecode(dataImage1);
        char buff[100];
        struct timeval tp;
        gettimeofday(&tp, NULL);
        long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

        sprintf(buff,"img%ld.jpg",ms);
        imwrite(buff, img1);
        shutdown(new_socket,SHUT_RD);
        close(new_socket);
        printf("Hello message sent\n");
    }   
    return 0;

}


    
  

#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage is:  " << argv[0] << " filename" << std::endl;
        return 1;
    }
    
    cv::VideoCapture cap;
    cap.open(argv[1]);
    
    cv::Mat frame;
    
    const char* WINDOW_NAME = "CV Video Player";
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    
    while(true)
    {
        cap >> frame;
        
        if (!frame.data)
        {
            std::cout << "Video over" << std::endl;
            break;
        }
        
        cv::imshow(WINDOW_NAME, frame);
        
        if (cv::waitKey(33) > 0)
        {
            std::cout << "User quit" << std::endl;
            break;
        }
    }
    
    std::cout << "Thank you for using my cool CV player to play " << argv[1] << std::endl;
    
}

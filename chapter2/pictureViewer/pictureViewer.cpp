#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage is:  " << argv[0] << " filename" << std::endl;
        return 1;
    }
    
    cv::Mat image = cv::imread(argv[1]);
    
    cv::imshow("picture", image);
    
    cv::waitKey(0);
}

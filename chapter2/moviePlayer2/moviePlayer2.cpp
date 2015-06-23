#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream> // ostringstream
#include <math.h> // isnan

int currentPosition = 0;

// Set to true when the onPositionCallback should not set the frame number
bool posCallbackNoSet = false;

cv::VideoCapture cap;

bool paused = false;

const char* WINDOW_NAME = "CV Video Player";
const char* TRACKBAR_NAME = "Movie Frame";

void onPositionCallback(int pos, void*)
{
    currentPosition = pos;
    
    if (!posCallbackNoSet)
    {
        // Set the frame
        std::cout << "Position set via trackbar to " << currentPosition << std::endl;
        cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage is:  " << argv[0] << " filename" << std::endl;
        return 1;
    }
        
    cap.open(argv[1]);
    
    int videoHeight = (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int videoWidth = (int) cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameCount = (int) cap.get(cv::CAP_PROP_FRAME_COUNT);
    double frameRate = cap.get(cv::CAP_PROP_FPS);
    
    if (isnan(frameRate))
    {
        std::cout << "Frame rate from file is invalid, assuming 30 fps" << std::endl;
        frameRate = 30.0;
    }
    
    std::cout << "Video dimensions: " << videoWidth << " x " << videoHeight << std::endl;
    std::cout << "Frame rate: " << frameRate << std::endl;
    std::cout << "Frame count: " << frameCount << " (" << frameCount / frameRate << " secs)" << std::endl;
    
    cv::Mat frame;
    
    
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, 0, frameCount, onPositionCallback, 0);
    
    
    bool runFlag = true;
    int captureIndex = 0;
    
    while(runFlag)
    {
        cap >> frame;
        
        
        if (!frame.data)
        {
            std::cout << "Video over" << std::endl;
            runFlag = false;
            continue;
        }
        
        cv::imshow(WINDOW_NAME, frame);
        
        int pos = (int) cap.get(cv::CAP_PROP_POS_FRAMES);
        if (paused)
        {
            pos--;
            cap.set(cv::CAP_PROP_POS_FRAMES, pos);
        }
            
        
        // Update position bar
        posCallbackNoSet = true;
        cv::setTrackbarPos (TRACKBAR_NAME, WINDOW_NAME, pos);
        posCallbackNoSet = false;
        
        int code = cv::waitKey(frameRate);
        if (code == -1)
        {
            // Nothing pressed
            continue;
        }
        
        switch( (char) code )
        {
        case 's':
        case 'p':
            // Stop
            std::cout << "Paused / Stopped" << std::endl;
            paused = true;
            break;
            
        case 'r':
            // Resume
            std::cout << "Resume" << std::endl;
            paused = false;
            break;
            
        case 'a':
        case 'b':
            // Backwards
            std::cout << "Backwards" << std::endl;
            paused = true;
            pos--;
            cap.set(cv::CAP_PROP_POS_FRAMES, pos);
            break;
            
        case 'd':
        case 'f':
            // Forwards
            std::cout << "Forwards" << std::endl;
            paused = true;
            pos++;
            cap.set(cv::CAP_PROP_POS_FRAMES, pos);
            break;
            
        case 'c':
            // Capture
            {
                std::ostringstream oss;
                oss << argv[1] << "_cap_" << captureIndex++ << ".jpg";
                std::cout << "Capture: " << oss.str() << std::endl;
                cv::imwrite(oss.str(), frame);
            }
            break;            
            
        default:
            // Any other key quits
            std::cout << "User quit.  Code=" << (char) code << std::endl;
            runFlag = false;
            break;
        
        }
     
    }
    
    std::cout << "Thank you for using my cool CV player to play " << argv[1] << std::endl;
    
}

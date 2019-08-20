// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
// Socket
#include "socket.hpp"

#define ImgHeight	480
#define ImgWidth	640

using namespace std;
using namespace cv;

int main()
{
	// Open camera
	int CamID = 0;
	VideoCapture cap(CamID); // open the camera
	Mat frame;
	if (!cap.isOpened()) // check if we succeeded
	{
		cout << "can't open the camera\n";
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, ImgWidth);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, ImgHeight);
	namedWindow("stream_client", WINDOW_AUTOSIZE);
	
	// --------------------------------------------------- //

	// build a client sockt
    SocketPair client(SocketPair::CLIENT);

	// buffer memory allocation
	client.RecvLen = 50 * sizeof(char);
	client.RecvBuffer = (char*)malloc(client.RecvLen);	// allocate the memory to char *

	// connect to server
    if ( !client.initial() )
        return 0;

	while (client.checkConnected())
	{
		cap >> frame; // get a new frame from camera
		//cout << "Cam width:" << frame.cols << ", height:" << frame.rows << endl;
		imshow("stream_client", frame);
		char keyIn = waitKey(1);	// press 'Esc' key to exit;
		if (keyIn == 27)
			break;

		// ------------------------------------------- //
		// set the message want to send to server
		frame = (frame.reshape(0,1)); // to make it continuous
		client.SendBuffer = (char*)frame.data;	// convert Mat to unsigned char*
		client.SendLen = frame.total()*frame.elemSize();
		// Send and receive the message 
		client.run();
		printf("Get from server: %s\n", client.RecvBuffer);
	}

	destroyAllWindows();
	return 0;
}

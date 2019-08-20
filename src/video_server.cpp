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
	// build a server socket
    SocketPair server(SocketPair::SERVER);
	server.waitRecvComplete = true;	// the image buffer is large, should wait untill complete.

	// buffer memory allocation
	server.RecvLen = ImgHeight*ImgWidth*3*sizeof(unsigned char);
	server.setRecvBuffer(server.RecvLen);
	server.SendLen = 50 * sizeof(char);
	server.setSendBuffer(server.SendLen);

	// set the message want to send to client
    char message[] = {"Hi, this is server."};
    strcpy(server.SendBuffer, message);

	// wait client connect
    if ( !server.initial() )
        return 0;

	// opencv variable
	Mat frame(ImgHeight, ImgWidth, CV_8UC3, (uchar *)server.RecvBuffer, Mat::AUTO_STEP); // convert char* to Mat (share the memory)
	namedWindow("stream_server", WINDOW_AUTOSIZE);

    while (server.checkConnected())
	{
		// Send and Receive the message
		server.run();

		// Show the image from client
		imshow("stream_server", frame);
		char keyIn = waitKey(1);	//press 'Esc' key to exit;
		if (keyIn == 27)
			break;
    }

	destroyAllWindows();
	return 0;
}
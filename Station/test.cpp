#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include"TransportProtocol.h"

using namespace std;
using namespace cv;
using namespace zbar;

int main(int argc, char **argv) {
    TransportProtocol TP;
    TP.TCPLink("127.0.0.1",8000);

    int cam_idx = 0;

    if (argc == 2) {
        cam_idx = atoi(argv[1]);
    }

    //VideoCapture cap(cam_idx);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH,480);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT,360);
    VideoCapture cap("./ordervideo.mp4");
    if (!cap.isOpened()) {
        cerr << "Could not open camera." << endl;
        exit(EXIT_FAILURE);
    }
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    namedWindow("captured", CV_WINDOW_AUTOSIZE);
    
    // Create a zbar reader
    ImageScanner scanner;
    
    // Configure the reader
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    int TCPsengcounter = 0;
    for (;;) {
        // Capture an OpenCV frame
        cv::Mat frame, frame_grayscale;
        cap >> frame;

        // Convert to grayscale
        cvtColor(frame, frame_grayscale, CV_BGR2GRAY);

        // Obtain image data
        int width = frame_grayscale.cols;
        int height = frame_grayscale.rows;
        uchar *raw = (uchar *)(frame_grayscale.data);

        // Wrap image data
        Image image(width, height, "Y800", raw, width * height);

        // Scan the image for barcodes
        //int n = scanner.scan(image);
        scanner.scan(image);

        // Extract results
        int counter = 0;
        string senddata;
        for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            //cout << "bar code is:" << endl << symbol->get_data() << endl;
            string codeinfor = symbol->get_data();
            senddata = "5,"+symbol->get_data();
            char *c_send = (char *)malloc(senddata.size()*sizeof(char)+1);
            for(int i = 0;i<senddata.size();i++)
                c_send[i] = senddata.at(i);
            c_send[senddata.size()] = '\n';
            if(TCPsengcounter%20 == 0)
                TP.TCPsend(c_send);
            cout<<"TCPsengcounter%20 : "<<TCPsengcounter%20<<endl;
            cout<<"TCPsengcounter : "<<TCPsengcounter++<<endl;
            //sleep(1);
            time_t now;
            tm *current;
            now = time(0);
            current = localtime(&now);

            // do something useful with results
            cout    << "[" << current->tm_hour << ":" << current->tm_min << ":" << setw(2) << setfill('0') << current->tm_sec << "] " << counter << " "
                    << "decoded " << symbol->get_type_name()
                    << " symbol \"" << symbol->get_data() << '"' << endl;

            //cout << "Location: (" << symbol->get_location_x(0) << "," << symbol->get_location_y(0) << ")" << endl;
            //cout << "Size: " << symbol->get_location_size() << endl;
            
            // Draw location of the symbols found
            if (symbol->get_location_size() == 4) {
                //rectangle(frame, Rect(symbol->get_location_x(i), symbol->get_location_y(i), 10, 10), Scalar(0, 255, 0));
                line(frame, Point(symbol->get_location_x(0), symbol->get_location_y(0)), Point(symbol->get_location_x(1), symbol->get_location_y(1)), Scalar(0, 255, 0), 2, 8, 0);
                line(frame, Point(symbol->get_location_x(1), symbol->get_location_y(1)), Point(symbol->get_location_x(2), symbol->get_location_y(2)), Scalar(0, 255, 0), 2, 8, 0);
                line(frame, Point(symbol->get_location_x(2), symbol->get_location_y(2)), Point(symbol->get_location_x(3), symbol->get_location_y(3)), Scalar(0, 255, 0), 2, 8, 0);
                line(frame, Point(symbol->get_location_x(3), symbol->get_location_y(3)), Point(symbol->get_location_x(0), symbol->get_location_y(0)), Scalar(0, 255, 0), 2, 8, 0);
            }
            
            // Get points
            /*for (Symbol::PointIterator point = symbol.point_begin(); point != symbol.point_end(); ++point) {
                cout << point << endl;
            } */
            counter++;
        }

        // Show captured frame, now with overlays!
        imshow("captured", frame);
                                                                                                                                                          
        // clean up
        image.set_data(NULL, 0);
        
        waitKey(30);
    }

    return 0;
}

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "include/helper.h"


using namespace cv;
using namespace std;

int main()
{
    model_to_bin("in/model.cdnnQdata", "out/model.bin"); // Write model to bin
    // Resize-------------------------------------------------
    Mat image; // Container for resized image
    string image_path = "in/input.jpg";
    string out_image = "out/image_with_classname.jpg";

    image = resize(image_path);
    //--------------------------------------------------------
    // Decode--------------------------------------------
    const char* resized_image = "out/resized.jpg";
    decode(resized_image);
    //Some IPC invokation to tell XM6 that decoded and resized image is in memory.... Host->XM6

    //-------------------------------------------------------------------
    // Write text to image
    //IPC message from XM6 to tell inference is done and host can retrieve the data(profiler, inference)...XM6->Host
    putText(image, "Burger", Point(10, image.cols / 5),
            FONT_HERSHEY_DUPLEX,
            1.0, CV_RGB(0,0,0), 2); // Get this string from XM6 side

    imwrite(out_image, image); // Write image with its class name
    printf("\nImage with class name written to out/\n");
    return 0;
}

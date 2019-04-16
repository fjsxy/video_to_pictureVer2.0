#include <iostream>
#include "CameraApi.h"//该头文件会在相机SDK安装后自动出现在/usr/include中
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>

using namespace std;
using namespace cv;

struct A{
    Mat frame;
    int ID;
};

void on_mouse(int event, int x,int y,int flags,void* ustc)
{
    A* a = static_cast<A*>(ustc);
    if (event == EVENT_LBUTTONDOWN)
    {
        char buffer[100];
        sprintf(buffer,"/home/fjsxy/camera_calibration/build/cal_images/image%d.jpg",a->ID);
        imwrite(buffer,a->frame);
        cout << "第" << a->ID+1 << "张图片已保存." << endl;
        a->ID++;
    }
}


int main() {
    A a;

    system("rm -rf /home/fjsxy/camera_calibration/build/cal_images/*");
    int cameraCount = 1;
    tSdkCameraDevInfo cameraEnumList;
    int cameraHandle;

    CameraEnumerateDevice(&cameraEnumList,&cameraCount);

    cout << "请输入曝光时间：" << endl;
    double e_time;
    cin >> e_time;
    a.ID = 0;
    tSdkCameraCapbility cameraCapbility;
    CameraInit(&cameraEnumList,-1,-1,&cameraHandle);
    CameraGetCapability(cameraHandle,&cameraCapbility);

    if (cameraCapbility.sIspCapacity.bMonoSensor)
        CameraSetIspOutFormat(cameraHandle, CAMERA_MEDIA_TYPE_MONO8);
    else
        CameraSetIspOutFormat(cameraHandle, CAMERA_MEDIA_TYPE_BGR8);

    BYTE* frame_buffer = (unsigned char *)malloc(cameraCapbility.sResolutionRange.iHeightMax *
                                                 cameraCapbility.sResolutionRange.iWidthMax * 3);
    CameraSetAeState(cameraHandle, false);
    CameraSetExposureTime(cameraHandle,e_time);
    CameraPlay(cameraHandle);

    int cvtype = CV_8UC1;
    BYTE* tmp_buffer;
    while(true) {
        tSdkFrameHead frameInfo;
        CameraGetImageBuffer(cameraHandle, &frameInfo, &tmp_buffer, 2000);
        CameraImageProcess(cameraHandle, tmp_buffer, frame_buffer, &frameInfo);
        CameraReleaseImageBuffer(cameraHandle, tmp_buffer);
        if (frameInfo.uiMediaType & CAMERA_MEDIA_TYPE_RGB)
            cvtype = CV_8UC3;
        a.frame = Mat(frameInfo.iHeight, frameInfo.iWidth, cvtype, frame_buffer);

        namedWindow("test");
        setMouseCallback("test", on_mouse,&a);
        imshow("test", a.frame);
        if (waitKey(30) > 0)
            return 0;
    }

    return 0;
}

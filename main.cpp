#include <iostream>
#include "MvCameraControl.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <CameraParams.h>

using namespace std;
using namespace cv;

struct A{
    Mat frame;
    int ID{};
    string save_path;
};

void on_mouse(int event, int x,int y,int flags,void* ustc)
{
    A* a = static_cast<A*>(ustc);
    if (event == EVENT_LBUTTONDOWN)
    {
        string imagePath = a->save_path + "/image" + to_string(a->ID) + ".jpg";
        imwrite(imagePath.c_str(),a->frame);
        cout << "第" << a->ID+1 << "张图片已保存." << endl;
        a->ID++;
    }
}


int main() {
    A a;
    a.ID = 0;

    float e_time = 20000;

    cout << "Please input exposure time:" << endl;
    cin >> e_time;

//    a.save_path = "file_path";
    cout << "Please input the path to save pictures(you should ensure your path is correct):" << endl;
    cin >> a.save_path;

    cout << "Do you want to delete all files in path[" << a.save_path << "], y for YES, n for NO[y or n]:" ;
    string checkByte;
    cin >> checkByte;
    if (checkByte == "y")
    {
        cout << "Begin to delete the old files..." << endl;
        string rm_command = "rm -rf " + a.save_path + "/*"; // rm -rf your_path/*
        system(rm_command.c_str());
        cout << "Delete complete!" << endl;
    }
    else if (checkByte == "n")
    {
        cout << "Don't delete the old files..." << endl;
        cout << "continue..." << endl;
    }


    // 枚举设备
    int nRet = MV_OK;
    MV_CC_DEVICE_INFO_LIST deviceInfoList;
    nRet = MV_CC_EnumDevices(MV_USB_DEVICE | MV_GIGE_DEVICE,&deviceInfoList);
    if (nRet != MV_OK)
    {
        cout << "Camera Enum Error! " << nRet << endl;
        return -1;
    }

    // 打印枚举设备信息
    cout << endl << "Device List:" << endl;
    for (int i = 0; i < deviceInfoList.nDeviceNum; ++i) {
        cout << "[device " << i << "]:" << endl;
        if (deviceInfoList.pDeviceInfo[i]->nTLayerType == MV_USB_DEVICE)
            cout << deviceInfoList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chModelName << endl;
        else if (deviceInfoList.pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
            cout << deviceInfoList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chModelName << endl;
        else
            cout << "Not Support." << endl;
    }

    // 选择设备
    int chooseID = 0;
    cout << "Please choose one cameraID you want:" << endl;
    while(cin >> chooseID)
    {
        if (chooseID >= deviceInfoList.nDeviceNum)
        {
            cout << "Input error. Please input ID again:" << endl;
        }
        else
            break;
    }

    // 创建设备句柄
    void *cameraHandle = nullptr;
    nRet = MV_CC_CreateHandle(&cameraHandle,deviceInfoList.pDeviceInfo[chooseID]);
    if (nRet != MV_OK)
    {
        cout << "Camera CreateHandle Error! " << nRet << endl;
        return -1;
    }

    // 打开设备
    nRet = MV_CC_OpenDevice(cameraHandle);
    if (nRet != MV_OK)
    {
        cout << "Camera OpenDevice Error! " << nRet << endl;
        return -1;
    }

    // 设置基本参数（目前只设置了曝光时间，可根据需要添加）
    nRet = MV_CC_SetExposureTime(cameraHandle,e_time);
    if (nRet != MV_OK)
    {
        cout << "Camera SetParameter Error! " << nRet << endl;
        return -1;
    }

    // 开始抓图
    nRet = MV_CC_StartGrabbing(cameraHandle);
    if (nRet != MV_OK)
    {
        cout << "Camera StartGrabbing Error! " << nRet << endl;
        return -1;
    }

    // 获取图像参数以及大循环
    MVCC_INTVALUE intParam;
    memset(&intParam, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(cameraHandle,"PayloadSize",&intParam); //获取int型中名为PayloadSize的参数
    if (nRet != MV_OK)
    {
        cout << "Camera Get PayloadSize Error! " << nRet << endl;
        return -1;
    }
    MV_FRAME_OUT_INFO_EX frameInfo = {0};
    memset(&frameInfo, 0 , sizeof(MV_FRAME_OUT_INFO_EX));
    auto* imageData = (unsigned char *)malloc(sizeof(unsigned char) * intParam.nCurValue);
    if (!imageData)
    {
        cout << "malloc failed!" << endl;
        return -1;
    }
    int dataSize = intParam.nCurValue;
    while(true)
    {
        nRet = MV_CC_GetOneFrameTimeout(cameraHandle,imageData,dataSize,&frameInfo,2000);
        if (nRet == MV_OK) {
            a.frame = Mat(frameInfo.nHeight,frameInfo.nWidth,CV_8UC1,imageData);
            namedWindow("show");
            setMouseCallback("show", on_mouse,&a);
            imshow("show", a.frame);
            if (waitKey(30) > 0)
                break;
        }
        else {
            cout << "NO image data!" << endl;
        }
    }

    // 释放存储图像内存空间
    cout << "free..." << endl;
    free(imageData);

    // 停止阶段
    cout << "stop..." << endl;
    nRet = MV_CC_StopGrabbing(cameraHandle);
    if (nRet != MV_OK)
    {
        cout << "Camera StopGrabbing Error! " << nRet << endl;
        return -1;
    }
    nRet = MV_CC_CloseDevice(cameraHandle);
    if (nRet != MV_OK)
    {
        cout << "Camera CloseDevice Error! " << nRet << endl;
        return -1;
    }
    nRet = MV_CC_DestroyHandle(cameraHandle);
    if (nRet != MV_OK)
    {
        cout << "Camera DestroyHandle Error! " << nRet << endl;
        return -1;
    }

    return 0;
}

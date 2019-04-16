# video_to_pictureVer1.0
该程序方便人们在linux下对工业相机采集的图片进行保存

###测试环境
ubuntu16.04
###说明
迈德威视USB工业相机获取视频流后人为保存图片的实现
###依赖
迈德威视SDK
opencv4.0

###How to use
```Bash
git clone https://github.com/fjsxy/video_to_pictureVer1.0.git
cd your_file/video_to_pictureVer1.0
mkdir build
cd build
cmake ..
make
./video_to_pic
```

###注意事项
连接上USB工业相机后，运行程序，在图中点击鼠标左键即可保存当前帧图片

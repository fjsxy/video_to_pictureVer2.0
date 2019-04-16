# video_to_pictureVer2.0
该程序方便人们在linux下对工业相机采集的图片进行保存

### 测试环境 ###
ubuntu16.04
### 说明 ###
海康威视USB工业相机获取视频流后人为保存图片的实现
### 依赖 ###
海康威视SDK  
opencv4.0  

### How to use ###
```Bash
git clone https://github.com/fjsxy/video_to_pictureVer2.0.git
cd your_file/video_to_pictureVer2.0
mkdir build
cd build
cmake ..
make
./Hik_video_to_pic
```

### 注意事项 ###
1、必须要保证保存图像的路径是正确的，最重要！！！  
2、可以选择删除或不删除路径中的老文件，主要是为了方便重新采集图片而舍弃老图片  
3、在保存完图像后退出窗口时务必采用esc，否则将导致内存泄漏

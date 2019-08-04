一）代码编译方法：
1）进入code-modify-public/build文件夹将所有文件删除
$cd /code-modify/build
$ls
$rm -rf *
2）cmake ..
3）make
4）./Demo
二）代码主要调试方向
1）主函数中包含三个线程：图片抓取线程t1、图片处理线程t2、数据接收线程t3
2）先调t1和t2线程，其中t1线程较为简单，源代码较为复杂可删减，基本思路为从视频中抓取一帧图像则保存到一个变量当中，并且只保存一幅图片
3）t2线程则每次则从保存图片的变量中读取图片，对图片进行处理，其中图片处理利用一些大神符已知尺寸来进行关节角度结算，确定云台电机应该如何旋转
4）调试t3线程，实现数据收发
三）需要修改内容
1）需要修改路径：
①main函数中config_file_name  对应的xml文件的路径需要修改
②xml文件里面对应的装甲模板（template_image_file small_template_image_file）路径需要修改
②xml文件里面对应的相机参数文件（intrinsic_file_480和intrinsic_file_720）路径需要修改
2)可能需要修改内容
打开相机部分（RMvedioCapture cap*****），包含两处位置，一个是在main.cpp中adjustExporesure函数中，另一个则在imageprodcsum.cpp里面，可能需要修改dev/vedio0改为dev/vedio1等
四）说明
代码不允许外传


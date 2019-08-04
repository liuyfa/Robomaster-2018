#CMake 最低版本要求
cmake_minimum_required (VERSION 2.8)
#项目信息
project(code-modify-public)

#添加内容
include(CheckCXXCompilerFlag)  
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)  
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)  
if(COMPILER_SUPPORTS_CXX11)  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")   
elseif(COMPILER_SUPPORTS_CXX0X)  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")  
else()  
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")  
endif()  

#寻找opencv库函数
find_package( OpenCV 3.4 REQUIRED )
#添加头文件
include_directories(${OpenCV_INCLUDE_DIRS})
#创建vediocapture库
add_library(VideoCapture 
AngleSolver.cpp    LedController.hpp  RemoteController.cpp    serial.cpp          
AngleSolver.hpp    main.cpp           RemoteController.hpp  RuneDetector.cpp   serial.h
ArmorDetector.cpp  cmdline.h          param_config.xml   RMVideoCapture.cpp    RuneDetector.hpp   Settings.hpp
ArmorDetector.hpp  ImageConsProd.cpp  Predictor.cpp      RMVideoCapture.hpp    RuneResFilter.cpp  
ImageConsProd.hpp  Predictor.hpp      RuneResFilter.hpp     sse_to_neon.hpp
#RMVision.pro	RMVision.pro.user

#cmdline.h                   
#RemoteController.hpp  
#LedController.hpp      
	)

#指定生成目标
add_executable(Demo main.cpp)

#链接opencv库
message("${OpenCV_LIBS}")
target_link_libraries(Demo ${OpenCV_LIBS})
target_link_libraries(Demo VideoCapture)
target_link_libraries(Demo pthread)

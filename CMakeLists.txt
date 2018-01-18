cmake_minimum_required(VERSION 2.8.9)
#project (time_image)
#project (video_to_bulb)
#project (txt_to_pic)
project (time_based_picture)
project (time_based_video)
project (real_time_video)

find_package(OpenCV)
#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "source/*.cpp")

#add_executable(time_image "./source/time_based.cpp")
#add_executable(video_to_bulb "./source/video.cpp")   
add_executable(time_based_picture "./source/time_based_picture.cpp" "./source/image_tool.cpp")
add_executable(time_based_video "./source/time_based_video.cpp" "./source/video_tool.cpp" "./source/image_tool.cpp")
#target_link_libraries(time_image ${OpenCV_LIBS} )  
target_link_libraries(time_based_picture ${OpenCV_LIBS} )
target_link_libraries(time_based_video ${OpenCV_LIBS} )
#target_link_libraries(video_to_bulb ${OpenCV_LIBS} )
#add_executable(txt_to_pic "./source/txt_to_pic.cpp")
#target_link_libraries(txt_to_pic ${OpenCV_LIBS} )

add_executable(real_time_video "./source/real_time_video.cpp")
target_link_libraries(real_time_video ${OpenCV_LIBS} )


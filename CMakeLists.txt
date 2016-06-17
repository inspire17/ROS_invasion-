cmake_minimum_required(VERSION 2.8.3)
project(test)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg )
find_package(PCL 1.7 REQUIRED)
## Declare ROS messages and services
#add_message_files(DIRECTORY msg FILES Num.msg)
#add_service_files(DIRECTORY srv FILES AddTwoInts.srv)

## Generate added messages and services
#generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} )
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS} ${PCL_DEFINITIONS})



add_executable(move_turtle turtlelinearly.cpp)
target_link_libraries(move_turtle ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} )
add_dependencies(move_turtle test_generate_messages_cpp)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS} ${PCL_DEFINITIONS})


add_executable(pub_pcl PC_publisher.cpp)
target_link_libraries(pub_pcl ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} )
add_dependencies(pub_pcl test_generate_messages_cpp)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS} ${PCL_DEFINITIONS})


add_executable(sub_pcl PC_sub.cpp)
target_link_libraries(sub_pcl ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${PCL_LIBRARIES} )
add_dependencies(sub_pcl test_generate_messages_cpp)



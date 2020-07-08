cmake_minimum_required(VERSION 2.8.3)
project(deneme)

find_package(catkin REQUIRED COMPONENTS roscpp geometry_msgs std_srvs turtlesim gazebo_msgs)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(pubvel src/pubvel.cpp)
target_link_libraries(pubvel ${catkin_LIBRARIES})

add_executable(returnBase src/returnBase.cpp)
target_link_libraries(returnBase ${catkin_LIBRARIES})

add_executable(regulateCmd src/regulateCmd.cpp)
target_link_libraries(regulateCmd ${catkin_LIBRARIES})

add_executable(setColor src/setColor.cpp)
target_link_libraries(setColor ${catkin_LIBRARIES})

add_executable(spawnNew src/spawnNew.cpp)
target_link_libraries(spawnNew ${catkin_LIBRARIES})

add_executable(directOther src/directOther.cpp)
target_link_libraries(directOther ${catkin_LIBRARIES})

add_executable(detectCollision src/detectCollision.cpp)
target_link_libraries(detectCollision ${catkin_LIBRARIES})

add_executable(spawnBox src/spawnBox.cpp)
target_link_libraries(spawnBox ${catkin_LIBRARIES})
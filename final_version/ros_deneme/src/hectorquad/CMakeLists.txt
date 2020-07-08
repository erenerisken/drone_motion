cmake_minimum_required(VERSION 2.8.3)
project(hectorquad)

find_package(catkin REQUIRED COMPONENTS roscpp geometry_msgs message_generation gazebo_msgs std_srvs) 

add_service_files(
  FILES
  coordinate.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(goPose src/goPose.cpp)
target_link_libraries(goPose ${catkin_LIBRARIES})

add_executable(rrtPlanner src/rrtPlanner.cpp)
target_link_libraries(rrtPlanner ${catkin_LIBRARIES})

add_executable(multipleGoPose src/multipleGoPose.cpp)
target_link_libraries(multipleGoPose ${catkin_LIBRARIES})

add_executable(publishPose src/publishPose.cpp)
target_link_libraries(publishPose ${catkin_LIBRARIES})

add_executable(subsVel src/subsVel.cpp)
target_link_libraries(subsVel ${catkin_LIBRARIES})

add_executable(avoidCollision src/avoidCollision.cpp src/Agent.cpp src/Obstacle.cpp)
target_link_libraries(avoidCollision ${catkin_LIBRARIES})

add_executable(measureVelocity src/measureVelocity.cpp)
target_link_libraries(measureVelocity ${catkin_LIBRARIES})

cmake_minimum_required(VERSION 2.8.3)
project(arduino_ros)

## 캐킨 빌드를 할 때 요구되는 구성요소 패키지이다.
## 의존성 패키지로 message_generation, std_msgs, roscpp이며 이 패키지들이 존재하지 않으면 빌드 도중에 에러가 난다.
find_package(catkin REQUIRED COMPONENTS message_generation std_msgs rospy roscpp)
## 메시지 선언: arduino.msg
add_message_files(FILES arduino.msg)

## 의존하는 메시지를 설정하는 옵션이다.
## std_msgs가 설치되어 있지 않다면 빌드 도중에 에러가 난다.
generate_messages(DEPENDENCIES std_msgs)
## 캐킨 패키지 옵션으로 라이브러리, 캐킨 빌드 의존성, 시스템 의존 패키지를 기술한다.
catkin_package(
INCLUDE_DIRS include
LIBRARIES arduino_ros
CATKIN_DEPENDS roscpp rospy std_msgs
)

## 인클루드 디렉터리를 설정한다.
include_directories(
	include
	${catkin_INCLUDE_DIRS}	
)

##헤더파일 추가
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

## topic_subscriber 노드에 대한 빌드 옵션이다.
## 실행 파일, 타깃 링크 라이브러리, 추가 의존성 등을 설정한다.
add_executable(arduino_sensor src/arduino_sensor.cpp)
add_dependencies(arduino_sensor ${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS})
target_link_libraries(arduino_sensor ${catkin_LIBRARIES})

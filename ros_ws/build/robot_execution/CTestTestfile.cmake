# CMake generated Testfile for 
# Source directory: /home/yuxuan/astra_robot_control/skill/ros_ws/src/robot_execution
# Build directory: /home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_validation "/usr/bin/python3" "-u" "/opt/ros/jazzy/share/ament_cmake_test/cmake/run_test.py" "/home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution/test_results/robot_execution/test_validation.gtest.xml" "--package-name" "robot_execution" "--output-file" "/home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution/ament_cmake_gtest/test_validation.txt" "--command" "/home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution/test_validation" "--gtest_output=xml:/home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution/test_results/robot_execution/test_validation.gtest.xml")
set_tests_properties(test_validation PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution/test_validation" TIMEOUT "60" WORKING_DIRECTORY "/home/yuxuan/astra_robot_control/skill/ros_ws/build/robot_execution" _BACKTRACE_TRIPLES "/opt/ros/jazzy/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/jazzy/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;95;ament_add_test;/opt/ros/jazzy/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/yuxuan/astra_robot_control/skill/ros_ws/src/robot_execution/CMakeLists.txt;27;ament_add_gtest;/home/yuxuan/astra_robot_control/skill/ros_ws/src/robot_execution/CMakeLists.txt;0;")
subdirs("gtest")

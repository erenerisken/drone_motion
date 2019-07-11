# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "hectorquad: 0 messages, 1 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(hectorquad_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" NAME_WE)
add_custom_target(_hectorquad_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hectorquad" "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(hectorquad
  "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hectorquad
)

### Generating Module File
_generate_module_cpp(hectorquad
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hectorquad
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(hectorquad_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(hectorquad_generate_messages hectorquad_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" NAME_WE)
add_dependencies(hectorquad_generate_messages_cpp _hectorquad_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hectorquad_gencpp)
add_dependencies(hectorquad_gencpp hectorquad_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hectorquad_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages

### Generating Services
_generate_srv_eus(hectorquad
  "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hectorquad
)

### Generating Module File
_generate_module_eus(hectorquad
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hectorquad
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(hectorquad_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(hectorquad_generate_messages hectorquad_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" NAME_WE)
add_dependencies(hectorquad_generate_messages_eus _hectorquad_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hectorquad_geneus)
add_dependencies(hectorquad_geneus hectorquad_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hectorquad_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(hectorquad
  "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hectorquad
)

### Generating Module File
_generate_module_lisp(hectorquad
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hectorquad
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(hectorquad_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(hectorquad_generate_messages hectorquad_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" NAME_WE)
add_dependencies(hectorquad_generate_messages_lisp _hectorquad_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hectorquad_genlisp)
add_dependencies(hectorquad_genlisp hectorquad_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hectorquad_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages

### Generating Services
_generate_srv_nodejs(hectorquad
  "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hectorquad
)

### Generating Module File
_generate_module_nodejs(hectorquad
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hectorquad
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(hectorquad_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(hectorquad_generate_messages hectorquad_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" NAME_WE)
add_dependencies(hectorquad_generate_messages_nodejs _hectorquad_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hectorquad_gennodejs)
add_dependencies(hectorquad_gennodejs hectorquad_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hectorquad_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(hectorquad
  "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hectorquad
)

### Generating Module File
_generate_module_py(hectorquad
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hectorquad
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(hectorquad_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(hectorquad_generate_messages hectorquad_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/burak/droneTask/src/hectorquad/srv/coordinate.srv" NAME_WE)
add_dependencies(hectorquad_generate_messages_py _hectorquad_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hectorquad_genpy)
add_dependencies(hectorquad_genpy hectorquad_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hectorquad_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hectorquad)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hectorquad
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(hectorquad_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hectorquad)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hectorquad
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(hectorquad_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hectorquad)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hectorquad
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(hectorquad_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hectorquad)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hectorquad
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(hectorquad_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hectorquad)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hectorquad\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hectorquad
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(hectorquad_generate_messages_py std_msgs_generate_messages_py)
endif()

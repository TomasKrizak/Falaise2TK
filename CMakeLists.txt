# Check cmake version meets our requirements
cmake_minimum_required(VERSION 3.3)

# Declare project, which will configure compiler for us
project(Falaise2TK)

#Finds ROOT and Falaise packages and defines cmake variables
find_package(Falaise REQUIRED)
find_package(ROOT    REQUIRED)

include_directories(include ${Bayeux_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp" "src/dicts/*.cpp")
add_library(Falaise2TK SHARED ${SOURCES} )

target_link_libraries(Falaise2TK PUBLIC Falaise::Falaise ${ROOT_LIBRARIES} ROOT::Geom)  

install(TARGETS Falaise2TK DESTINATION ${CMAKE_INSTALL_PLUGINDIR})

configure_file("Falaise2TK.conf.in" "Falaise2TK.conf" @ONLY)

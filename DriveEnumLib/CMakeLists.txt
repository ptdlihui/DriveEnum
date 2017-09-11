add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
include_directories(../include)
add_library(DriveEnumLib STATIC driveenumimp.cpp service.h service.cpp)
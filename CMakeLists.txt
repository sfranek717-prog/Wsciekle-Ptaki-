cmake_minimum_required(VERSION 3.16)

project(Wsciekle_Ptaki LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)
add_subdirectory(box2d)
add_executable(Wsciekle_Ptaki
    main.cpp
    obiekt.h 
    silnik_obrazen.h silnik_obrazen.cpp
     
)


target_link_libraries(Wsciekle_Ptaki 
    PRIVATE 
        SFML::Graphics 
        SFML::Window 
        SFML::System
        SFML::Audio
        box2d
)
target_include_directories(Wsciekle_Ptaki PRIVATE box2d/include)
include(GNUInstallDirs)
install(TARGETS Wsciekle_Ptaki
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(MSVC)
    add_compile_options(/utf-8)
endif()
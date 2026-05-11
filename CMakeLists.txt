cmake_minimum_required(VERSION 3.16)

project(Wsciekle_Ptaki LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

add_executable(Wsciekle_Ptaki
    main.cpp
    silnik_fizyczny.h silnik_fizyczny.cpp
    obiekt.h obiekt.cpp
    silnik_obrazen.h silnik_obrazen.cpp
    ksztalty.h ksztalty.cpp
)

target_link_libraries(Wsciekle_Ptaki 
    PRIVATE 
        SFML::Graphics 
        SFML::Window 
        SFML::System
        SFML::Audio
)

include(GNUInstallDirs)
install(TARGETS Wsciekle_Ptaki
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
cmake_minimum_required(VERSION 3.20)
project(TheLockScreenQuestion)

set(CMAKE_CXX_STANDARD 14)

set(sfml_window_dir
        sfml_window/window.h
        sfml_window/window.cpp
        sfml_window/view.h
        sfml_window/view.cpp
        )

add_executable(TheLockScreenQuestion  main.cpp
        ${sfml_window_dir}
        lock.cpp
        lock.h)

add_subdirectory(SFML)
add_subdirectory(pm-stl)

set(executables TheLockScreenQuestion)

foreach(executable IN LISTS executables)
    target_link_directories(${executable}
            PUBLIC ${PROJECT_SOURCE_DIR}/sfml/src
            PUBLIC ${PROJECT_SOURCE_DIR}/pm-stl
            )

    target_link_libraries(${executable}
            sfml-graphics
            sfml-system
            sfml-window
            pm_stl
            )

    target_include_directories(${executable}
            PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include
            PUBLIC ${PROJECT_SOURCE_DIR}/pm-stl
            )
endforeach()

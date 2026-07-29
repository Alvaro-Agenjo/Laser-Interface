cmake_minimum_required(VERSION 3.16)

project(Ag_Laser VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}) # Not needed for CMake >= 2.8.11


set(TS_FILES Ag_Laser_es_ES.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        laserview.cpp
        laserview.h

        laserscene.cpp
        laserscene.h

        gcodegenerator.cpp
        gcodegenerator.h



        proceso.cpp
        proceso.h

        factoria.cpp
        factoria.h

        operacion.cpp
        operacion.h

        forma.cpp
        forma.h

        PuntosPaso.h



        forma_geometrica.cpp
        forma_geometrica.h

        forma_rectangulo.cpp
        forma_rectangulo.h


        forma_compleja.cpp
        forma_compleja.h

        forma_imagen.cpp
        forma_imagen.h

        forma_texto.cpp
        forma_texto.h





        SP_test.cpp
        SP_test.h
        SP_test.ui

        cut_test.cpp
        cut_test.h
        cut_test.ui



        PerfilMaterial.h

        perfilesdlg.cpp
        perfilesdlg.h
        perfilesdlg.ui

        text_test.h
        text_test.cpp
        text_test.ui

        forma_png.h
        forma_png.cpp



        recursos.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Ag_Laser MANUAL_FINALIZATION ${PROJECT_SOURCES}
            forma_elipse.h forma_elipse.cpp
            CONSTANTS.h)

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(Ag_Laser ${PROJECT_SOURCES})

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Ag_Laser PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

target_include_directories(Ag_Laser PRIVATE ${OpenCV_INCLUDE_DIRS})
# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Ag_Laser)
# endif()
# set_target_properties(Ag_Laser PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# include(GNUInstallDirs)
# install(TARGETS Ag_Laser
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(Ag_Laser)
# endif()

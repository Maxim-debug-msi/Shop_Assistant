cmake_minimum_required(VERSION 3.20)
project(shop_assistant)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Users/Maxim/Desktop/Programming/Qt/5.15.2/mingw81_64/lib/cmake")

set(SOURCES main.cpp UI/MainWindow.cpp objectTree_model.cpp data/data.cpp
        UI/DataMatrixList.cpp
        UI/dialogues/addGroup_dlg.cpp utilities.cpp UI/dialogues/deleteGroup_dlg.cpp
        stb_image_write.cpp UI/ProductTables.cpp UI/ProductCard.cpp UI/LabelThermalPrinter.cpp
        UI/dialogues/searchProduct_dlg.cpp UI/BarcodeScanner.cpp)

set(LIBS ../include/UI/MainWindow/MainWindow.h ../include/models/objectTree_model.h
        ../include/utilities.h ../include/UI/DataMatrixList/DataMatrixList.h
        ../include/UI/dialogues/addGroup_dlg.h ../include/UI/dialogues/deleteGroup_dlg.h
        ../include/stb_image_write.h ../include/UI/productTables/ProductTables.h ../include/UI/productCard/ProductCard.h
        ../include/UI/thermalPrinter/LabelThermalPrinter.h ../include/impls/implData.h ../include/UI/dialogues/searchProduct_dlg.h
        ../include/defs.h ../include/UI/Devices/BarcodeScanner/BarcodeScanner.h ../include/impls/implDevices.h
        )

set(UI ../include/UI/productTables/ProductTables.ui ../include/UI/MainWindow/MainWindow.ui
        ../include/UI/productCard/productCard.ui ../include/UI/thermalPrinter/thermalPrinter.ui
        ../include/UI/DataMatrixList/DataMatrixList.ui ../include/UI/Devices/BarcodeScanner/BarcodeScanner.ui)

set(ICONS ../icons/icons.qrc)

add_subdirectory(barcodeGen)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        Sql
        SerialPort
        REQUIRED
        )

add_executable(shop_assistant WIN32 ${SOURCES} ${LIBS} ${UI} ${ICONS})

target_link_libraries(shop_assistant
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::PrintSupport
        Qt5::Sql
        Qt5::SerialPort
        ZXing
        )

if (WIN32)
    set(DEBUG_SUFFIX)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qminimal${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    if(EXISTS "${QT_INSTALL_PATH}/plugins/printsupport/windowsprintersupport${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/printsupport/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/printsupport/windowsprintersupport${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/printsupport/")
    endif()

    if(EXISTS "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
    endif()

    if(EXISTS "${QT_INSTALL_PATH}/plugins/imageformats/qsvg${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/imageformats/qsvg${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
    endif()

    if(EXISTS "${QT_INSTALL_PATH}/plugins/imageformats/qico${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/imageformats/qico${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/imageformats/qtiff${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
    endif()

    foreach (QT_LIB Core Gui Widgets PrintSupport Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

target_include_directories(shop_assistant PUBLIC ../include/)
target_include_directories(shop_assistant PUBLIC ../include/impls/)
target_include_directories(shop_assistant PUBLIC ../include/UI/productTables/)
target_include_directories(shop_assistant PUBLIC ../include/UI/MainWindow/)
target_include_directories(shop_assistant PUBLIC ../include/UI/productCard/)
target_include_directories(shop_assistant PUBLIC ../include/UI/thermalPrinter/)
target_include_directories(shop_assistant PUBLIC ../include/UI/dialogues/)
target_include_directories(shop_assistant PUBLIC ../include/UI/DataMatrixList/)
target_include_directories(shop_assistant PUBLIC ../include/UI/Devices/BarcodeScanner/)
target_include_directories(shop_assistant PUBLIC ../include/forms/)




cmake_minimum_required(VERSION 3.15)
project(Anilab VERSION 1.0.0 DESCRIPTION "The ultimate anime bot for all servers")

aux_source_directory(src mainsrc)
aux_source_directory(src/cmd_common common)
aux_source_directory(src/cmd_moderation moderation)
aux_source_directory(src/database database)

add_executable(${PROJECT_NAME} ${mainsrc} ${common} ${moderation} ${database} "Anilab.rc")

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/dpp.lib)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/sqlite3.lib)
target_include_directories(${PROJECT_NAME} PRIVATE "cores" "include" "include/dpp" "lib")

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>

    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/.env $<TARGET_FILE_DIR:${PROJECT_NAME}>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/languages $<TARGET_FILE_DIR:${PROJECT_NAME}>/languages

    COMMAND ${CMAKE_COMMAND} -E make_directory 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/database
)
cmake_minimum_required(VERSION 3.16)
project(app 
    VERSION 1.0.0
    DESCRIPTION "Codeforces API client"
    LANGUAGES CXX
)

# Добавляем путь к нашим CMake модулям
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Подключаем наши модули
include(CompilerOptions)
include(Dependencies)
include(SourceFiles)
include(Testing)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройки сборки
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Опции проекта
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(ENABLE_STATIC_ANALYSIS "Enable static analysis" OFF)

# Сбор исходных файлов
collect_sources(PROJECT_SOURCES)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Настройка включаемых директорий
setup_include_directories(${PROJECT_NAME})

# Настройка зависимостей
setup_all_dependencies(${PROJECT_NAME})

# Настройка опций компилятора
set_compiler_options(${PROJECT_NAME})

# Установка выходной директории
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "codeforces_client"
)

# Создание директории для вывода
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Информация о сборке
message(STATUS "=== Build Configuration ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "===========================")

# Добавляем custom target для очистки
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "Cleaning all build artifacts"
)

# Подключаем настройки пакетирования в конце
include(Package)
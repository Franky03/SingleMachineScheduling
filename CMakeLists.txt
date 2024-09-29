# Versão mínima do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(opt)

# Definir o padrão de compilação para C++ 17 ou posterior
set(CMAKE_CXX_STANDARD 17)

# Definir o tipo de build padrão para Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)

# Definir os diretórios de inclusão (caminho para headers)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/logs)
include_directories(${PROJECT_SOURCE_DIR}/src/algorithms)

# Adicionar todos os arquivos .cpp em src, algorithms, models e utils
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/algorithms/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/models/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/algorithms/neighbor.cpp"
    "${PROJECT_SOURCE_DIR}/src/algorithms/solver.cpp"
)

# Opção para habilitar o profiling com gprof
option(ENABLE_PROFILING "Enable profiling with gprof" OFF)

if(ENABLE_PROFILING)
    message(STATUS "Profiling enabled: adding -pg and -g flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

# Adicionar o executável
add_executable(opt ${SRC_FILES})

# Ligar o executável às bibliotecas de threads
target_link_libraries(opt ${CMAKE_THREAD_LIBS_INIT})

# Definir flags específicas de compilação para otimização no modo Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

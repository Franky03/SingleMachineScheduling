# Versão mínima do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(opt)

# Definir o padrão de compilação para C++ 17 ou posterior
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

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

# Adicionar o executável
add_executable(opt ${SRC_FILES})
target_compile_options(opt PRIVATE -O3)
target_link_libraries(opt ${CMAKE_THREAD_LIBS_INIT})
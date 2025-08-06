cmake_minimum_required(VERSION 4.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" CACHE STRING "")
set(SHADER_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/_Shaders")

project(GNRProject CXX)
set(TARGET_NAME GNR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
include(cmake/copy_dll.cmake)
include(cmake/get_cpm.cmake)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS filesystem system thread locale REQUIRED)
CPMAddPackage(
    NAME NRIFramework
    GITHUB_REPOSITORY NVIDIA-RTX/NRIFramework
    GIT_TAG 57b30eb
)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} 
Eigen3::Eigen
Boost::system
Boost::filesystem
Boost::dynamic_linking
Boost::thread
Boost::locale
NRIFramework
)

copy_dll(${TARGET_NAME})



# Compile options
if(NOT((CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64") OR(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")))
    set(SIMD -mssse3)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMPILE_OPTIONS ${SIMD} -Wextra -Wno-missing-field-initializers)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(COMPILE_OPTIONS ${SIMD} -Wextra -Wno-missing-field-initializers)
elseif(MSVC)
    set(COMPILE_OPTIONS /W4 /WX
        /wd4324 # padding was added at the end of a structure because you specified an alignment specifier
    )
else()
    message(WARNING "Unknown compiler!")
endif()

# Compile definitions
if(WIN32)
    set(COMPILE_DEFINITIONS WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)
endif()
target_compile_definitions(${TARGET_NAME} PRIVATE ${COMPILE_DEFINITIONS} PROJECT_NAME=${TARGET_NAME})
target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_OPTIONS})

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${PROJECT_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# External/NRIFramework
set(NRI_SHADERS_PATH "${SHADER_OUTPUT_PATH}" CACHE STRING "")

# Shaders
file(GLOB_RECURSE SHADERS "Shaders/*.hlsl" "Shaders/*.hlsli")
set_source_files_properties(${SHADERS} PROPERTIES VS_TOOL_OVERRIDE "None")

get_target_property(ML_SOURCE_DIR MathLib SOURCE_DIR)
get_target_property(NRI_SOURCE_DIR NRI SOURCE_DIR)

set(SHADERMAKE_GENERAL_ARGS
    --project "NRISamples"
    --compactProgress
    --binary
    --flatten
    --stripReflection
    --WX
    --sRegShift 0 --bRegShift 32 --uRegShift 64 --tRegShift 128 # TODO: must be in sync with "VK_BINDING_OFFSETS"
    --sourceDir "Shaders"
    --ignoreConfigDir
    -c "Shaders/Shaders.cfg"
    -o "${SHADER_OUTPUT_PATH}"
    -I "Shaders"
    -I "${ML_SOURCE_DIR}"
    -I "${NRI_SOURCE_DIR}/Include"
)

set(SHADERMAKE_COMMANDS "")

if(NRI_ENABLE_D3D11_SUPPORT)
    set(SHADERMAKE_COMMANDS ${SHADERMAKE_COMMANDS} COMMAND ${SHADERMAKE_PATH} -p DXBC --compiler "${SHADERMAKE_FXC_PATH}" ${SHADERMAKE_GENERAL_ARGS})
endif()

if(NRI_ENABLE_D3D12_SUPPORT)
    set(SHADERMAKE_COMMANDS ${SHADERMAKE_COMMANDS} COMMAND ${SHADERMAKE_PATH} -p DXIL --compiler "${SHADERMAKE_DXC_PATH}" ${SHADERMAKE_GENERAL_ARGS})
endif()

if(NRI_ENABLE_VK_SUPPORT)
    set(SHADERMAKE_COMMANDS ${SHADERMAKE_COMMANDS} COMMAND ${SHADERMAKE_PATH} -p SPIRV --compiler "${SHADERMAKE_DXC_VK_PATH}" ${SHADERMAKE_GENERAL_ARGS})
endif()

add_custom_target(${TARGET_NAME}Shaders ALL
    ${SHADERMAKE_COMMANDS}
    DEPENDS ShaderMake
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
    SOURCES ${SHADERS}
)

set_target_properties(${TARGET_NAME}Shaders PROPERTIES FOLDER ${PROJECT_NAME})

add_dependencies(${TARGET_NAME} ${TARGET_NAME}Shaders)

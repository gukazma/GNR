set(TARGET_NAME GNREditor)


file(GLOB_RECURSE CPP_HEAD_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE CPP_SOUCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(SHADER_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/_Shaders")
file(GLOB_RECURSE SHADERS "Shaders/*.hlsl" "Shaders/*.hlsli")

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${CPP_HEAD_FILES} ${CPP_SOUCE_FILES})


add_executable(${TARGET_NAME} ${CPP_HEAD_FILES} ${CPP_SOUCE_FILES})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Apps)
target_link_libraries(${TARGET_NAME} PRIVATE
NRIFramework
NRI
spdlog::spdlog
)

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Shaders

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
    --PDB
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

add_custom_target(${PROJECT_NAME}Shaders ALL
    ${SHADERMAKE_COMMANDS}
    DEPENDS ShaderMake
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
    SOURCES ${SHADERS}
)

set_target_properties(${PROJECT_NAME}Shaders PROPERTIES FOLDER ${PROJECT_NAME})


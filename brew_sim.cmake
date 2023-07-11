cmake_minimum_required(VERSION 3.17)

set(BREW_TOOLCHAIN_FILE_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(BREW_PLATFORM_SDK_ROOT "${BREW_TOOLCHAIN_FILE_DIR}/platform_sdk_root/sdk")

set(CMAKE_SYSROOT fs:/)
set(CMAKE_STAGING_PREFIX fs:/)

include_directories(${BREW_PLATFORM_SDK_ROOT}/inc)

function(add_brew_module module_name)
    add_library(${module_name} SHARED ${ARGN})
	target_compile_definitions(${module_name} PRIVATE -DDYNAMIC_APP -DAEE_SIMULATOR)
	set_property(TARGET ${module_name} PROPERTY
             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endfunction()

#set(CMAKE_C_FLAGS "-fshort-wchar -mword-relocations -ffunction-sections -fdata-sections -fno-exceptions -marm -mthumb-interwork -march=armv5te -specs=nosys.specs")
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-use-cxa-atexit -fno-rtti")
#set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,--entry=AEEMod_Load -Wl,--emit-relocs -Wl,--default-script=${CMAKE_CURRENT_LIST_DIR}/elf2mod.x -Wl,--no-wchar-size-warning -static-libgcc -Wl,--gc-sections")

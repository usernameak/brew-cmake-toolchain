cmake_minimum_required(VERSION 3.17)

set(BREW_TOOLCHAIN_FILE_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(BREW_COMPILER_ROOT_PATH "${BREW_TOOLCHAIN_FILE_DIR}/gcc_root")
set(BREW_PLATFORM_SDK_ROOT "${BREW_TOOLCHAIN_FILE_DIR}/platform_sdk_root/sdk")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT fs:/)
set(CMAKE_STAGING_PREFIX fs:/)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_C_COMPILER ${BREW_COMPILER_ROOT_PATH}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${BREW_COMPILER_ROOT_PATH}/bin/arm-none-eabi-g++)
if (WIN32)
    set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER}.exe)
    set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER}.exe)
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

include_directories(${BREW_PLATFORM_SDK_ROOT}/inc)

function(add_brew_module module_name)
    add_executable(${module_name} ${ARGN})
	target_compile_definitions(${module_name} PRIVATE -DDYNAMIC_APP)
    add_custom_command(TARGET ${module_name} POST_BUILD COMMAND
        ${BREW_TOOLCHAIN_FILE_DIR}/elf2mod.exe 
        ARGS    -output $<TARGET_FILE:${module_name}>.mod 
                $<TARGET_FILE:${module_name}>)
endfunction()

set(CMAKE_C_FLAGS "-fshort-wchar -mword-relocations -ffunction-sections -fdata-sections -fno-exceptions -marm -mthumb-interwork -march=armv5te -specs=nosys.specs -D__ARMCGCC")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-use-cxa-atexit -fno-rtti")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,--entry=AEEMod_Load -Wl,--emit-relocs -Wl,--default-script=${CMAKE_CURRENT_LIST_DIR}/elf2mod.x -Wl,--no-wchar-size-warning -static-libgcc -Wl,--gc-sections")

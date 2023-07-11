cmake_minimum_required(VERSION 3.17)

project(helloworld)

add_brew_module(helloworld
        ${BREW_PLATFORM_SDK_ROOT}/src/AEEModGen.c
        ${BREW_PLATFORM_SDK_ROOT}/src/AEEAppGen.c

        helloworld.c
        helloworld.bid
)

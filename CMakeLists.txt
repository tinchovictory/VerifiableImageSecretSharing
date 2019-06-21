cmake_minimum_required(VERSION 3.5)
project(VerifiableImages C)




set(CMAKE_C_FLAGS    "--std=c99 -pthread -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -D_POSIX_C_SOURCE=200112L ")

set(CLANG_TIDY_CHECKS *
   -boost-*
   -mpi-*
   -llvm-*
   -cppcoreguidelines-*
   -clang-analyzer-osx.*
   -cert-*-cpp
   -misc-unused-parameters,
   -clang-diagnostic-missing-declarations)
string(REPLACE ";" "," CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS}")

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    find_program(CLANGTIDY NAMES clang-tidy)
    if(CLANGTIDY)
        message(STATUS "Found clang-tidy: ${CLANGTIDY}")
        set(CMAKE_C_CLANG_TIDY clang-tidy
                -checks=${CLANG_TIDY_CHECKS}
                -warnings-as-errors=*
                -fix)
    endif()
endif()

set(CMAKE_C_FLAGS, "${CMAKE_C_FLAGS} -pthread -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -D_POSIX_C_SOURCE=200112L")


add_executable(testMatrix
                utils/matrix/testMatrix.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c)

add_executable(testGaussElimination
                utils/gaussElimination/testGaussElimination.c
                utils/gaussElimination/gaussElimination.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c)

add_executable(encrypter
                encrypter/testEncrypter.c
                encrypter/encrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c)

add_executable(decrypter
                decrypter/testDecrypter.c
                decrypter/decrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c)

add_executable(encrypterDecrypter
                testEncrypterDecrypter.c
                encrypter/encrypter.c
                decrypter/decrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c)

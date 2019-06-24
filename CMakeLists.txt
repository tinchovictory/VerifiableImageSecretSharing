cmake_minimum_required(VERSION 3.5)
project(VerifiableImages C)




set(CMAKE_C_FLAGS    "--std=c99 -lm -pthread -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -D_POSIX_C_SOURCE=200112L ")

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

set(CMAKE_C_FLAGS, "${CMAKE_C_FLAGS} -lm -pthread -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -D_POSIX_C_SOURCE=200112L -D_BSD_SOURCE")


add_executable(testMatrix EXCLUDE_FROM_ALL
                utils/matrix/testMatrix.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/gaussElimination/gaussElimination.c)

add_executable(testGaussElimination EXCLUDE_FROM_ALL
                utils/gaussElimination/testGaussElimination.c
                utils/gaussElimination/gaussElimination.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c)

add_executable(encrypter EXCLUDE_FROM_ALL
                encrypter/testEncrypter.c
                encrypter/encrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/random/random.c
                utils/gaussElimination/gaussElimination.c)

add_executable(decrypter EXCLUDE_FROM_ALL
                decrypter/testDecrypter.c
                decrypter/decrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c)

add_executable(encrypterDecrypter EXCLUDE_FROM_ALL
                tests/testEncrypterDecrypter.c
                encrypter/encrypter.c
                decrypter/decrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c
                utils/random/random.c)

add_executable(bmpReader EXCLUDE_FROM_ALL
                bmpReader/testBmpReader.c
                bmpReader/bmpReader.c)

add_executable(stenography EXCLUDE_FROM_ALL
                stenography/testStenography.c
                stenography/stenography.c)

add_executable(decryptImage EXCLUDE_FROM_ALL
                tests/testDecrypt.c
                decrypter/decrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c
                bmpReader/bmpReader.c
                stenography/stenography.c
                utils/random/random.c)

add_executable(encryptImage EXCLUDE_FROM_ALL
                tests/testEncrypt.c
                encrypter/encrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c
                bmpReader/bmpReader.c
                stenography/stenography.c
                utils/random/random.c)

add_executable(main
                main.c
                encrypter/encrypter.c
                decrypter/decrypter.c
                utils/matrix/matrix.c
                utils/multiplicativeInverse/multiplicativeInverse.c
                utils/matrixArray/matrixArray.c
                utils/gaussElimination/gaussElimination.c
                bmpReader/bmpReader.c
                stenography/stenography.c
                utils/random/random.c
                paramParser/paramParser.c)

target_link_libraries(main m)
target_link_libraries(encryptImage m)
target_link_libraries(encrypter m)
target_link_libraries(encrypterDecrypter m)

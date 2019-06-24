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

set(CMAKE_C_FLAGS, "${CMAKE_C_FLAGS} -lm -pthread -pedantic -pedantic-errors -Wall -Wextra -Werror -Wno-unused-parameter -D_POSIX_C_SOURCE=200112L -D_BSD_SOURCE")


add_executable(testMatrix EXCLUDE_FROM_ALL
                src/utils/matrix/testMatrix.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/gaussElimination/gaussElimination.c)

add_executable(testGaussElimination EXCLUDE_FROM_ALL
                src/utils/gaussElimination/testGaussElimination.c
                src/utils/gaussElimination/gaussElimination.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c)

add_executable(encrypter EXCLUDE_FROM_ALL
                src/encrypter/testEncrypter.c
                src/encrypter/encrypter.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/matrixArray/matrixArray.c
                src/utils/random/random.c
                src/utils/gaussElimination/gaussElimination.c)

add_executable(decrypter EXCLUDE_FROM_ALL
                src/decrypter/testDecrypter.c
                src/decrypter/decrypter.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/matrixArray/matrixArray.c
                src/utils/gaussElimination/gaussElimination.c)

add_executable(encrypterDecrypter EXCLUDE_FROM_ALL
                src/tests/testEncrypterDecrypter.c
                src/encrypter/encrypter.c
                src/decrypter/decrypter.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/matrixArray/matrixArray.c
                src/utils/gaussElimination/gaussElimination.c
                src/utils/random/random.c)

add_executable(bmpReader EXCLUDE_FROM_ALL
                src/bmpReader/testBmpReader.c
                src/bmpReader/bmpReader.c)

add_executable(stenography EXCLUDE_FROM_ALL
                src/stenography/testStenography.c
                src/stenography/stenography.c)

add_executable(decryptImage EXCLUDE_FROM_ALL
                src/tests/testDecrypt.c
                src/decrypter/decrypter.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/matrixArray/matrixArray.c
                src/utils/gaussElimination/gaussElimination.c
                src/bmpReader/bmpReader.c
                src/stenography/stenography.c
                src/utils/random/random.c)

add_executable(encryptImage EXCLUDE_FROM_ALL
                src/tests/testEncrypt.c
                src/encrypter/encrypter.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/matrixArray/matrixArray.c
                src/utils/gaussElimination/gaussElimination.c
                src/bmpReader/bmpReader.c
                src/stenography/stenography.c
                src/utils/random/random.c)

add_executable(main
                src/main.c
                src/encrypter/encrypter.c
                src/decrypter/decrypter.c
                src/utils/matrix/matrix.c
                src/utils/multiplicativeInverse/multiplicativeInverse.c
                src/utils/matrixArray/matrixArray.c
                src/utils/gaussElimination/gaussElimination.c
                src/bmpReader/bmpReader.c
                src/stenography/stenography.c
                src/utils/random/random.c
                src/paramParser/paramParser.c)

target_link_libraries(main m)
target_link_libraries(encryptImage m)
target_link_libraries(encrypter m)
target_link_libraries(encrypterDecrypter m)

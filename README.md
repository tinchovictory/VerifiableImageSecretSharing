# Verifiable image secret sharing

This is a project for exploring secret sharing of messages in images

## Getting started

### Prerequisites

 CMake: 

```
$ apt-get install cmake
```

If Clang is installed it will compile with it. Otherwise it will use GCC:

```
$ apt-get install clang
```


## Building 

Create a directory for building the files 

```
$ mkdir build && cd build
```

Build the project with cmake
```
$ cmake ..
```

Make the target
```
$ make
```
## Running

Run the compiled output for encrypting

```
./main -d -s secret1.bmp -m watermark.bmp -k 2 -n 4 -dir ../grupo8/2-4/ -rw ../grupo8/2-4W/RW.bmp
```

Run the compiled output for decrypting

```
./main -r -s secret1.bmp -m watermark.bmp -k 2 -n 4 -dir ../grupo8/2-4/ -rw ../grupo8/2-4W/RW.bmp
```

**Note:** The algorithm is not decrypting the secret image correctly when using the higher index shares of the *(4, 8)* scheme.
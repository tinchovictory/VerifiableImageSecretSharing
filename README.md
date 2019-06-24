# Verifiable image secret sharing

This is a project for exploring secret sharing of messages in images

## Getting started

### Prerequisites

 CMake: 

```
$> brew install cmake
```

Clang:

```
$> brew install clang
```


### Building 

Create a directory for building the files 

```
$> mkdir build && cd build
```

Build the project with cmake
```
$> cmake ..
```

Make the target 
```
make
```
### Running

Run the compiled output

```
./main -r -s secret1.bmp -m watermark.bmp -k 2 -n 4 -dir ../grupo8/2-4/ -rw ../grupo8/2-4W/RW.bmp
```

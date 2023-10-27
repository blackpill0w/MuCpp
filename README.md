# MuCpp
A music player written in C++20 & Qt6.
# Dependenies
Cmake will take care of everything, but I am not very good with it, so it might fail :)
- [cmake](https://cmake.org)
- [conan2](https://conan.io)
- [Qt6](https://qt.io)
# Build
Make sure your compiler supports the `std::format()` function,
you can verify here (in the *Text formatting* line) <https://en.cppreference.com/w/cpp/compiler_support/20>.<br>
Run this from the root directory of the project.
```bash
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```

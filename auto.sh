mkdir build
cd build
conan install .. --build missing
cmake .. -G "Xcode"
cmake --build .
cd bin
./year2graphics
cd ../..

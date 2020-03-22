cd ../build
cmake ..
mingw32-make
cd ../lib
g++ -o main.exe main.cpp -I../include -L./ -lnew-graphics.dll
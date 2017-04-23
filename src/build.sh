mkdir -p ../build
g++ -std=c++14 -O2 -o ../build/gameoflife main.cpp -lsfml-graphics -lsfml-window -lsfml-system
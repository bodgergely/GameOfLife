mkdir -p ../build
rm ../build/gameoflife
g++ -std=c++14 -g -O2 -o ../build/gameoflife main.cpp -pthread -lsfml-graphics -lsfml-window -lsfml-system
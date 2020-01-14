echo Compiling...
g++ -c main.cpp -o compiledFiles/main.o
echo Done!

if [ "$1" == "-r" ]
 then
  echo Compiling with external libraries...
  g++ compiledFiles/main.o Flock.cpp Boid.cpp BoidUtils.cpp Utils.cpp -o compiledFiles/flocking-debug -lsfml-graphics -lsfml-window -lsfml-system
  echo Running..
  compiledFiles/flocking-debug
fi

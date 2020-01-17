echo Compiling...
g++ -c main.cpp -o main.o
echo Done!

if [ "$1" == "-r" ]
 then
  echo Compiling with external libraries...
  g++ main.o ../Flocking/Flock.cpp ../Flocking/Boid.cpp ../Flocking/BoidUtils.cpp ../Utils/Utils.cpp -o animation.out -lsfml-graphics -lsfml-window -lsfml-system
  echo Running..
  ./animation.out
fi

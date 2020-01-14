echo Compiling...
g++ mainForPi.cpp Flock.cpp Boid.cpp BoidUtils.cpp Utils.cpp ws2812-rpi.cpp -o flocklights -fpermissive
echo Done!

if [ "$1" == "-r" ]
 then
  echo Running..
  ./flocklights
fi

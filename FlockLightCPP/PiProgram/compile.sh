echo Compiling...
g++ main.cpp ../Flocking/Flock.o ../Utils/Utils.cpp ../Utils/ws2812-rpi.o -o flocklights -fpermissive
echo Done!

if [ "$1" = "-r" ]
 then
  echo Running..
  ./flocklights
fi

echo Compiling...
g++ main.cpp ../Utils/ws2812-rpi.o -o LEDSelector -fpermissive
echo Done!

if [ "$1" = "-r" ]
 then
  echo Running..
  ./LEDSelector
fi

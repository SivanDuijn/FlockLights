run=false;

while [ "$1" != "" ]; do
    case $1 in
        -r | --run )  
            run=true
        ;;
        -ca | --compileAll | -all )
            echo Compiling external libraries to object files
            cd ..
            ./compileAllDep.sh
            cd PiProgram/
        ;;
    esac
    shift
done

echo Compiling...
g++ main.cpp ../Flocking/Flock.o ../Utils/Utils.cpp ../Utils/ws2812-rpi.o -o flocklights -fpermissive

if [ $run = "true" ]
    then
        echo Running..
        ./flocklights
fi

echo Done!

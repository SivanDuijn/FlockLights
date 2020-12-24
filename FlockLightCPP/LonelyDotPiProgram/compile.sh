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
            cd LonelyDotPiProgram/
        ;;
    esac
    shift
done

echo Compiling...
g++ main.cpp ../Utils/Utils.cpp ../Utils/ws2812-rpi.o -o run -fpermissive

if [ $run = "true" ]
    then
        echo Running..
        ./run
fi

echo Done!

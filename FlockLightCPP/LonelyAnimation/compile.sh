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
            cd Animation/
        ;;
    esac
    shift
done

echo Compiling...
g++ main.cpp ../Flocking/Flock.o ../Utils/Utils.cpp -o animation.out -lsfml-graphics -lsfml-window -lsfml-system

if [ $run = "true" ]
    then
        echo Running..
        ./animation.out
fi

echo Done!
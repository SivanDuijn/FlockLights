while [ "$1" != "" ]; do
    case $1 in
        -f | --file )           echo hahhaaa;;
        -i | --interactive )    echo hi
                                ;;
        -h | --help )           usage
                                exit
                                ;;
        * )                     usage
                                exit 1
    esac
    shift
done
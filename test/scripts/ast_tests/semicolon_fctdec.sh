i=0
function func()
{
    case $i in
        0)i=1; echo i is now $i;;
        1)i=2; echo i is now $i;;
        *)i=2;;
    esac
}

func; func;

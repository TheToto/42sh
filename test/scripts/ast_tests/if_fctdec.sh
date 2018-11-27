i=0

function func()
{
    if test $i -eq 0; then
        i=1
    else
        i=0
    fi
}

function all()
{
    case $i in
    0) echo true;;
    1) echo false;;
    esac
    func
}

all
all
all
all

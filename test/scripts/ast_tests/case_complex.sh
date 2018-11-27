i=0

function func()
{
    case $i in
        0)i=1;;
        1)i=2;;
        2)i=3;;
        *)i=4;;
    esac
}

while ! test $i -eq 4; do
    func
    echo Iteration $i
done

i=1

function func1()
{
    echo Hello ACU!
}

function func2()
{
    echo Hello ING!
}
function func3()
{
    i=0
    if test $i; then
        func1
    else
        func2
    fi
}
func3

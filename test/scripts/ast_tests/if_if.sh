i=0
j=1
if test $i; then
    if test $j; then
        echo all true
    else
        echo true and false
    fi
else
    if test $j; then
        echo false and true;
    else
        echo all false
    fi
fi

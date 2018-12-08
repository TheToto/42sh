i=$RANDOM
j=$RANDOM
k=$RANDOM
l=$RANDOM
m=$RANDOM
a=$((i + j * l))
b=$((i + k * m))
if test $a -eq $b; then
    echo KO
else
    echo OK
fi

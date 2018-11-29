i=0
while ! test $i -eq 3; do
    echo This should appear only once
    i=3
done

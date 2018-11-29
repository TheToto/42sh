i=0

for j in 1 2 3 4 5 6;do
    case $j in
    1) ls;;
    2) pwd;;
    3) tree;;
    4) echo Hello World > hi
    *) echo Hello World
done

while test $i; do
    echo hello again
    i=1
done
cat < in
rm hi

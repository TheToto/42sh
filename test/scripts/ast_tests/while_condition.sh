i=1
while test $i -eq 1;
do
    echo This should appear only once
    i=0;
done
printf $i;

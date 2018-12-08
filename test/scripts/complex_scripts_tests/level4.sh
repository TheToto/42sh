i=0
while [ $i -lt 10 ]
do
    x=$RANDOM
    echo $x >> coucou
    i=$((i+1))
done
echo $(cat coucou | wc -l)
rm coucou

for i in 1 2 3; do for j in 1 2 3; do if [ $i -eq 2 ]; then break 2; fi;
echo $j; done; echo $i; done

while true; do for i in 1 2 3 4 5; do if [ $i -eq 4 ]; then break 2; fi;
echo $i; done; done

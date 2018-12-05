for i in 1 2 3 4; do for j in 5 6 7 8; do if [ $j -eq 5 ]; then continue; fi;
echo $j; done; echo $i; done

for i in 1 2 3 4 5; do if [ $i -eq 3 ]; then continue; fi; echo $i; done

#i=0
#while [ $i -ne 10 ]; do if [ $i -eq 5 ]; then continue; fi; ; echo $i; done

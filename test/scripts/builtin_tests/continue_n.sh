for i in 1 2 3 4 5 6; do for j in 1 2 3; do if [ $i -eq 4 ]; then continue 2;
fi; echo $i; done; done

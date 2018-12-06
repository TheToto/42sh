while true; do while ! false; do for i in 1 2 3 4 5 6;
do if [ $i -eq 4 ]; then break 5; fi; echo $i; done; done; done;

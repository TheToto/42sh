test() {
    ls;
} > file 2> file2
test
echo AFTER THIS;
cat file;
rm file;
rm file2;

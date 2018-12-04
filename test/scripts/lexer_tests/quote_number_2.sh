test2() { test $1; }
test() { echo I have $# args; }

test2 "Hello toi";

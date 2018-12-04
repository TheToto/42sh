test() { echo "I have $# args"; }
test b "o" $nope "$nope"
test '' "" $nope

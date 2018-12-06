export -n '_'

hello=world
export ola=$hello
export| wc -l > nb
cat nb
rm nb

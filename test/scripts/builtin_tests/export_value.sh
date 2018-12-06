export -n '_'
export coucou="Hello world"
export | wc -l > nb
cat nb
rm nb

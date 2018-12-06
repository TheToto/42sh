export -n '_'
coucou="test"
export -p | wc -l > nb
cat nb
rm nb

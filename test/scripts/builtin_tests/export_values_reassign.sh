export -n '_'

export hello="world"
export coucou="salut"
export | wc -l > nb
cat nb

export coucou="ola"
export | wc -l > nb
cat nb

export coucou=""
export coucou
export| wc -l > nb
cat nb
rm nb

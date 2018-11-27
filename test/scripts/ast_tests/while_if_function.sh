i=1
function answer()
{
    if test $i -eq 1; then
        return 0
    else
        return 1
    fi
}

while answer; do
    i=0
    echo This should appear only once
done

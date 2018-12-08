function hello()
{
    i=0
    while ! test $i -eq $#;do
        echo \$$i
        i=$((i + 1))
    done
}

hello one two three
hello This is "a test"

function test_arg()
{
    echo $#
}

a="hello-lovely-world"
b="hello lovely world"
test_arg $a
IFS='-'
test_arg $a
test_arg $b

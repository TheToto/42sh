function test_arg()
{
    echo $#
}

a="hello-lovely-world"
b="hello lovely world"
test_arg "hello-lovely-world"
IFS='-'
test_arg "hello-lovely-world"
test_arg "hello lovely world"

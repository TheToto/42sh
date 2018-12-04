test2()
{
    echo 2 : $#;
}
test()
{
    echo 1 : $#;
    test2 $*;
    test2 $@;
}
test "un deux" trois

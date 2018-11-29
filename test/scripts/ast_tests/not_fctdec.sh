function func()
{
    return 0
}

if ! test func; then
    echo failed
else
    echo success
fi

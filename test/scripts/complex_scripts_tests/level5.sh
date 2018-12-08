function hey()
{
case "$1" in
    abc) echo "string = abc"
    ;;
    xyz) echo "string = xyz"
    ;;
    *)   echo "not abc, not zyz" ;;
esac
}

hey abc
hey xyz
hey dsfsdf

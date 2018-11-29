function test() {
    echo this is my test;
};
if test;
then
    for i in 1 2 3;
    do
        ls /;
    done
else
    while true;
    do
        ls;
    done
fi

lol=salut;
case $lol in
    sal) echo nope;;
    sal*) echo yep;;
    *) echo all;;
esac

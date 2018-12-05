echo "for i in 1 2 3;do
echo \$i
exit
echo bite" > salut
source salut
rm salut

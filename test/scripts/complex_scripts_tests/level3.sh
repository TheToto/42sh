function iter()
{
if ! test $# -eq 1;then
  exit 1
fi
if test $1 -eq 0;then
  echo 1
  exit 0
fi

echo $1
echo $2
exit 0
}

iter 5 2
iter 2 3

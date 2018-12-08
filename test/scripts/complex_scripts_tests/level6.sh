function testing()
{
if ! test $# -eq 1;then
  exit 1
fi

while IFS='' read -r line;do
{
  COUNT=$(echo $line | wc -m)
  if test $COUNT -gt 80;then
    echo $line
  fi
}
done < "$1"
}
echo testestestsetestestsettestestestestettestetstettstetststtetstetsttstetetstetsttetstettetstetstetdtstetst > test1
echo testtest > test2
testing test1
testing test2
rm test1
rm test2

for i in one two three; do
    case $i in
    one) echo This is iteration 1 ;;
    two) echo This is iteration 2 ;;
    three) echo This is iteration 3 ;;
    *) echo Unknown iteration;;
    esac
done

function coucou()
{
    echo "Using \"\$*\":"
        for a in "$*"; do
            echo $a;
    done

        echo -e "\nUsing \$*:"
        for a in $*; do
            echo $a;
    done

        echo -e "\nUsing \"\$@\":"
        for a in "$@"; do
            echo $a;
    done

        echo -e "\nUsing \$@:"
        for a in $@; do
            echo $a;
    done
}

coucou one two "three four"

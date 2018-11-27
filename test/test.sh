#!/bin/sh

GREEN="\033[1;92m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
ANNONCE="\033[38;5;37m"
DEFAULT="\033[1;39m"
RESET="\033[0;39m"

##############################################################################
#                               OPTION PARSING                               #
##############################################################################

if test "$1" = "check"; then
    shift
    cd ..
fi

list_of_category="ast_tests lexer_tests parser_tests option_tests"
timeout="10000d"
sanity=0

while test $# -gt 0; do
    case $1 in
        -c | --category)
            list_of_category=""
            shift
            if test "$1" = ";"; then
                printf $RED"\nERROR: no category: one category at least should be tested\n\n"$DEFAULT
                exit 1
            fi
            while test $# -gt 0 -a "$1" != ";"; do
                case $1 in
                    ast | lexer | parser | option);;
                    * )
                        printf $RED"\nERROR: invalid category: $1\n\n"$DEFAULT
                        exit 1;;
                esac
                list_of_category="$list_of_category $1""_tests"
                shift
            done
            if test "$1" != ";"; then
                printf $RED"\nERROR: category list should finish with \'\;\'\n\n"
                exit 1;
            fi;;
        -l | --list)
            printf $ANNONCE"\n  list of categories:\n\t  - ast\n\t  - lexer\n\t  - option\n\t  - parser\n\n"$DEFAULT;
            exit 0;;
        -t | --timeout)
            shift
            is_correct=$(echo $1 | /bin/grep -E "^[0-9]*(\.[0-9])?[0-9]*$")
            if [ -n  "$is_correct" ]; then
                timeout="$1""s"
            else
                printf $RED"\nERROR: Invalid time: $1\n\n"$DEFAULT
                exit 1
            fi;;
        -s | --sanity)
            sanity=1;;
        * )
            printf $RED"\nERROR: invalid option: $1\n\n"$DEFAULT
            exit 1;;
    esac
    shift
done

is_in () {
    for item in $2; do
        if [ $1 = $item ]; then
            return 0
        fi
    done
    return 1
}

##############################################################################
#                                  COMPIL'                                   #
##############################################################################

printf $DEFAULT$ANNONCE"---------"$DEFAULT"\n"
printf $YELLOW"  BEGIN"$DEFAULT"\n"
printf $ANNONCE"---------"$DEFAULT"\n\n"

printf $ANNONCE"  ----------------\n"$DEFAULT
printf $YELLOW"  BUILDING PROJECT\n"$DEFAULT
printf $ANNONCE"  ----------------\n\n"$DEFAULT

cd build

printf $ANNONCE"    Cleaning ...\n"$DEFAULT

make clean

printf $ANNONCE"    Building ...\n"

make -B > /dev/null 2> /dev/null

printf $ANNONCE"    COMPLETE!\n"$DEFAULT

cd ..

##############################################################################
#                                  UNITARY                                   #
##############################################################################

list_of_dir="$(ls test/unitary)"

list_of_file=""

for dir in $list_of_dir; do
    is_in $dir "$list_of_category"
    is_asked=$?
    if [ $is_asked -eq 0 ]; then
        dir_path="test/unitary/$dir"
        for file in $(ls "$dir_path"); do
            list_of_file="$list_of_file"" test:$dir_path""/$file"
        done
    fi
done

printf "$list_of_life"

ceedling $list_of_file > tmp 2> tmp_err

printf $ANNONCE"\n  -------------"$DEFAULT"\n"
printf $YELLOW"  UNITARY TESTS"$DEFAULT"\n"
printf $ANNONCE"  -------------"$DEFAULT"\n\n"

is_err=0

while read line; do
    if [ $is_err -eq 1 ]; then
        char_count="$(echo "$line" | wc -m)"
        if [ $char_count -gt 1 -a "$line" != "-------------------" ]; then
            printf "      $line\n"
            continue
        elif [ "$line" != "-------------------" ]; then
            printf "\n\n"
            is_err=0
        fi
    fi

    case $line in
    *BEGINNINGEND*)
              printf "      "$GREEN"PASSED"$DEFAULT"\n";;
    *BEGINNING* )
              printf "      "$RED"FAILED"$DEFAULT"\n";;
    *-Testing:*   )
              test_comment="$(echo "$line" | sed -r 's/.*\"([^\"]*)\".*/\1/g')"
              printf "    "$YELLOW"$test_comment"$DEFAULT"\n";;
    "FAILED TEST"*)
              printf "    \n"$RED"-------------------\n"
              printf "    $line\n"
              printf "    -------------------\n\n"
              is_err=1;;
    TESTED* ) printf "\n    "$YELLOW"$line"$DEFAULT"\n\n";;
    PASSED* ) printf "    "$GREEN"$line"$DEFAULT"\n";;
    FAILED* ) printf "    "$RED"$line"$DEFAULT"\n";;
    IGNORED*) printf "    "$DEFAULT"$line"$DEFAULT"\n";;
    *       )
    esac
done < tmp
rm tmp tmp_err

##############################################################################
#                                  GLOBAL                                    #
##############################################################################

pretty_printf_err () {
    printf $RED"      FAILED: differences between bash and 42sh\n        < 42sh\n        > bash\n\n"$DEFAULT
    while read line; do
        printf $RED"      $line\n"$DEFAULT
    done < "$1"
}

check_sanity () {
    while read line; do
        case $line in
            *"LEAK SUMMARY"* )
                return 0;;
            * )
                continue;;
        esac
    done < /tmp/tmp_sanity
    return 1
}

list_of_dir="$(ls test/scripts)"

list_of_file=""

for dir in $list_of_dir; do
    is_in $dir "$list_of_category"
    is_asked=$?
    if [ $is_asked -eq 0 ]; then
        dir_path="test/scripts/$dir"
        for file in $(ls "$dir_path"); do
            list_of_file="$list_of_file"" $dir_path""/$file"
        done
    fi
done

printf "\n"$ANNONCE"  ------------"$DEFAULT"\n"
printf $YELLOW"  GLOBAL TESTS"$DEFAULT"\n"
printf $ANNONCE"  ------------"$DEFAULT"\n\n"

TESTED=0
PASSED=0
FAILED=0

for file in $list_of_file; do
    TESTED="$(($TESTED + 1))"
    printf "    -"$YELLOW"Testing $file file"$DEFAULT"-\n"
    bash "$file" 2> /tmp/tmp_ref_err | cat -e > /tmp/tmp_ref
    build/42sh "$file" 2> /tmp/tmp_def_err | cat -e > /tmp/tmp_def

    exit_status=0
    if [ $timeout != "10000d" ]; then
        timeout $timeout build/42sh "$file" > /dev/null 2> /dev/null
        exit_status="$?"
    fi
    
    exit_status_sanity=1
    if [ $sanity -eq 1 ]; then
        valgrind build/42sh "$file" 2> /tmp/tmp_sanity > /tmp/null
        check_sanity
        exit_status_sanity="$?"
        rm /tmp/tmp_sanity
    fi

    diff /tmp/tmp_def /tmp/tmp_ref > /tmp/res
    diff_content="$(cat /tmp/res)"
    if [ $exit_status_sanity -eq 0 ]; then
        FAILED="$(($FAILED + 1))"
        printf $RED"      FAILED: Leaks\n\n"$DEFAULT
    elif [ $exit_status -eq 124 ]; then
        FAILED="$(($FAILED + 1))"
        printf $RED"      FAILED: Timeout\n\n"$DEFAULT
    elif [ -n "$diff_content" ]; then
        FAILED="$(($FAILED + 1))"
        pretty_printf_err /tmp/res
        printf "\n"
    else
        PASSED="$(($PASSED + 1))"
        printf $GREEN"      PASSED\n"
    fi

    rm /tmp/tmp_r*
    rm /tmp/tmp_d*
    rm /tmp/res
done

printf $ANNONCE"    ----------------"$DEFAULT"\n"
printf $YELLOW"    RESULTS (GLOBAL)"$DEFAULT"\n"
printf $ANNONCE"    ----------------"$DEFAULT"\n\n"

printf $YELLOW"      TESTED:  $TESTED\n\n"$DEFAULT
printf $GREEN"      PASSED:  $PASSED\n"$DEFAULT
printf $RED"      FAILED:  $FAILED\n\n"$DEFAULT

printf $ANNONCE"-------"$DEFAULT"\n"
printf $YELLOW"  END"$DEFAULT"\n"
printf $ANNONCE"-------"$RESET"\n\n"


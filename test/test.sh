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

list_of_category="ast_tests lexer_tests parser_tests option_tests builtin_tests quote_tests arithmetic_tests"
timeout="10000d"
sanity=0

while test $# -gt 0; do
    case $1 in
        -c | --category)
            list_of_category=""
            shift
            if test "$1" = ";"; then
                printf $RED"\nERROR: no category: one category at least should be tested\n\n"$RESET
                exit 1
            fi
            while test $# -gt 0 -a "$1" != ";"; do
                case $1 in
                    ast | lexer | parser | option | builtin | quote | arithmetic );;
                    * )
                        printf $RED"\nERROR: invalid category: $1\n\n"$RESET
                        exit 1;;
                esac
                list_of_category="$list_of_category $1""_tests"
                shift
            done
            if test "$1" != ";"; then
                printf $RED"\nERROR: category list should finish with \'\;\'\n\n"$RESET
                exit 1;
            fi;;
        -l | --list)
            printf $ANNONCE"\n  list of categories:\n\t  - arithmetic\n\t  - ast\n\t  - builtin\n\t  - lexer\n\t  - option\n\t  - parser\n\t  - quote\n\n"$DEFAULT;
            exit 0;;
        -t | --timeout)
            shift
            is_correct=$(echo $1 | /bin/grep -E "^[0-9]*(\.[0-9])?[0-9]*$")
            if [ -n  "$is_correct" ]; then
                timeout="$1""s"
            else
                printf $RED"\nERROR: Invalid time: $1\n\n"$RESET
                exit 1
            fi;;
        -s | --sanity)
            sanity=1;;
        * )
            printf $RED"\nERROR: invalid option: $1\n\n"$RESET
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

pretty_printf_dash () {
    nb="$(echo $1 | wc -m)"
    while [ $nb -gt 1 ]; do
        printf $ANNONCE"-"$DEFAULT
        nb="$(($nb - 1))"
    done
}

pretty_printf_category () {
    dir_name="$(echo $1 | cut -f 3 -d / | sed -e 's/_/ /g' | sed 's/.*/\U&/')"
    printf $ANNONCE"\n    " #---------"
    pretty_printf_dash "$dir_name"
    printf $DEFAULT"\n    $dir_name\n"
    printf $ANNONCE"    " #---------"
    pretty_printf_dash "$dir_name"
    printf "\n\n"$DEFAULT
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

did_it_works="$(find . -name 42sh)"

if test -z $did_it_works; then
    printf $RED"    ERROR: Error while compiling: you may try to compile before running test-suite\n\n"$RESET
    exit 1
fi

printf $ANNONCE"    COMPLETE!\n"$DEFAULT

cd ..

##############################################################################
#                                  UNITARY                                   #
##############################################################################

TESTED_U=""
PASSED_U=""
FAILED_U=""
IGNORED_U=""

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
current_dir_test=""
TEST=""

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
              printf "      "$GREEN"PASSED"$DEFAULT": $TEST"$DEFAULT"";;
    *BEGINNING* )
              printf "      "$RED"FAILED"$DEFAULT": $TEST"$DEFAULT"";;
    *-Testing:*   )
              test_dir="$(echo "$line" | cut -f 1 -d : | cut -f 3 -d /)"
              if [ "$test_dir" != "$current_dir_test" ]; then
                current_dir_test="$test_dir"
                pretty_printf_category "$line"
              fi
              test_comment="$(echo "$line" | sed -r 's/.*\"([^\"]*)\".*/\1/g')"
              TEST=$YELLOW"$test_comment"$DEFAULT"\n";;
    "FAILED TEST"*)
              printf "\n    "$RED"-------------------\n"
              printf "    $line\n"
              printf "    -------------------\n\n"
              is_err=1;;
    TESTED* ) printf $ANNONCE"    -------"$DEFAULT"\n"
              printf $YELLOW"    RESULTS"$DEFAULT"\n"
              printf $ANNONCE"    -------"$DEFAULT"\n\n"

              TESTED_U="      "$YELLOW"$line"$DEFAULT"\n\n"
              printf "$TESTED_U";;
    PASSED* ) PASSED_U="      "$GREEN"$line"$DEFAULT"\n"
              printf "$PASSED_U";;
    FAILED* ) FAILED_U="      "$RED"$line"$DEFAULT"\n"
              printf "$FAILED_U";;
    IGNORED*) IGNORED_U="      "$DEFAULT"$line"$DEFAULT"\n"
              printf "$IGNORED_U";;
    *       )
    esac
done < tmp
rm tmp tmp_err

##############################################################################
#                                  GLOBAL                                    #
##############################################################################

pretty_printf_err () {
    printf $RED"      FAILED"$DEFAULT": $TEST\n"
    printf $RED"        differences between bash and 42sh\n          < 42sh\n          > bash\n\n"$DEFAULT
    while read line; do
        printf $RED"        $line\n"$DEFAULT
    done < "$1"
}

pretty_printf_stderr () {
    printf $RED"      STDERR DEF:\n"$DEFAULT
    echo "$1" > /tmp/stderr_tmp
    while read line; do
        printf $RED"      *   $line\n"$DEFAULT
    done < /tmp/stderr_tmp
    printf "\n"
    
    printf $RED"      STDERR REF:\n"$DEFAULT
    echo "$2" > /tmp/stderr_tmp
    while read line; do
        printf $RED"      *   $line\n"$DEFAULT
    done < /tmp/stderr_tmp
    printf "\n"
    
    rm /tmp/stderr_tmp
}

check_sanity () {
    no_error=2
    while read line; do
        case $line in
            *"LEAK SUMMARY"* )
                return 0;;
            *"ERROR SUMMARY: 0 errors"* )
                no_error=1;;
            * )
                continue;;
       esac
    done < /tmp/tmp_sanity
    return $no_error
}

list_of_dir="$(ls test/scripts)"

list_of_file=""

for dir in $list_of_dir; do
    is_in $dir "$list_of_category"
    is_asked=$?
    if [ $is_asked -ne 1 ]; then
        dir_path="test/scripts/$dir"
        list_of_file="$list_of_file"" $dir_path"
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
    if [ -d $file ]; then
        pretty_printf_category "$file"
        continue
    fi
    TESTED="$(($TESTED + 1))"
    TEST=$DEFAULT"    -"$YELLOW"Testing $file file"$DEFAULT"-\n"
    bash --posix "$file" 2> /tmp/tmp_ref_err > /tmp/tmp 
    exit_status_ref="$?"

    cat -e /tmp/tmp > /tmp/tmp_ref
    rm /tmp/tmp
    
    timeout $timeout build/42sh "$file" 2> /tmp/tmp_def_err > /tmp/tmp
    exit_status="$?"

    cat -e /tmp/tmp > /tmp/tmp_def
    rm /tmp/tmp

    exit_status_sanity=1
    if [ $sanity -eq 1 ]; then
        res_sanity="$(valgrind build/42sh $file 2>&1)"
        echo "$res_sanity" > /tmp/tmp_sanity
        check_sanity
        exit_status_sanity="$?"
        rm /tmp/tmp_sanity
    fi

    diff /tmp/tmp_def /tmp/tmp_ref > /tmp/res
    diff_content="$(cat /tmp/res)"
    if [ $exit_status_sanity -ne 1 ]; then
        FAILED="$(($FAILED + 1))"
        printf $RED"      FAILED"$DEFAULT": $TEST"
        if [ $exit_status_sanity -eq 0 ]; then
            printf $RED"        Leaks\n"$DEFAULT
        else
            printf $RED"        Errors found\n"$DEFAULT
        fi
    elif [ $exit_status -eq 124 ]; then
        FAILED="$(($FAILED + 1))"
        printf $RED"      FAILED"$DEFAULT": $TEST"
        printf $RED"        Timeout\n"$DEFAULT
    elif [ -n "$diff_content" ]; then
        FAILED="$(($FAILED + 1))"
        pretty_printf_err /tmp/res
        printf "\n"
    elif [ $exit_status -ne $exit_status_ref ]; then
        FAILED="$(($FAILED + 1))"
        printf $RED"      FAILED"$DEFAULT": $TEST\n"
        printf $RED"        Invalid return value: expected $exit_status_ref got $exit_status\n"
        pretty_printf_stderr "$(cat /tmp/tmp_def_err)" "$(cat /tmp/tmp_ref_err)"
    else
        PASSED="$(($PASSED + 1))"
        printf $GREEN"      PASSED"$DEFAULT": $TEST"
    fi

    rm /tmp/tmp_r*
    rm /tmp/tmp_d*
    rm /tmp/res
done

printf $ANNONCE"    -------"$DEFAULT"\n"
printf $YELLOW"    RESULTS"$DEFAULT"\n"
printf $ANNONCE"    -------"$DEFAULT"\n\n"

printf $YELLOW"      TESTED:  $TESTED\n\n"$DEFAULT
printf $GREEN"      PASSED:  $PASSED\n"$DEFAULT
printf $RED"      FAILED:  $FAILED\n\n"$DEFAULT

printf $ANNONCE"  -------"$DEFAULT"\n"
printf $YELLOW"  SUMMARY"$DEFAULT"\n"
printf $ANNONCE"  -------"$DEFAULT"\n\n"

printf $DEFAULT"    -UNITARY-\n\n"

printf "$TESTED_U"
printf "$PASSED_U"
printf "$FAILED_U"

printf $DEFAULT"\n    -GLOBAL-\n\n"

printf $YELLOW"      TESTED:  $TESTED\n\n"$DEFAULT
printf $GREEN"      PASSED:  $PASSED\n"$DEFAULT
printf $RED"      FAILED:  $FAILED\n\n"$DEFAULT

printf $ANNONCE"-------"$DEFAULT"\n"
printf $YELLOW"  END"$DEFAULT"\n"
printf $ANNONCE"-------"$RESET"\n\n"

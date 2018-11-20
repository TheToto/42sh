#!/bin/sh

GREEN="\033[1;92m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
ANNONCE="\033[38;5;37m"
DEFAULT="\033[1;39m"

printf $ANNONCE"---------"$DEFAULT"\n"
printf $YELLOW"  BEGIN"$DEFAULT"\n"
printf $ANNONCE"---------"$DEFAULT"\n\n"

##############################################################################
#                                  COMPIL'                                   #
##############################################################################

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
#                                  UNITAR                                    #
##############################################################################

ceedling > tmp 2> tmp_err

printf $ANNONCE"\n  ------------"$DEFAULT"\n"
printf $YELLOW"  UNITAR TESTS"$DEFAULT"\n"
printf $ANNONCE"  ------------"$DEFAULT"\n\n"

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
    *-Testing:*   )
              test_comment="$(echo "$line" | sed -r 's/.*\"([^\"]*)\".*/\1/g')"
              printf "    "$YELLOW"$test_comment"$DEFAULT"\n";;
    "FAILED TEST"*)
              printf "    "$RED"-------------------\n"
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
    while read line; do
        printf $RED"      $line\n"$DEFAULT
    done < "$1"
}

printf "\n"$ANNONCE"  ------------"$DEFAULT"\n"
printf $YELLOW"  GLOBAL TESTS"$DEFAULT"\n"
printf $ANNONCE"  ------------"$DEFAULT"\n\n"

cd test

TESTED=0
PASSED=0
FAILED=0

for file in $(ls scripts); do
    TESTED="$(($TESTED + 1))"
    printf "    -"$YELLOW"Testing $file file"$DEFAULT"-\n"
    bash "scripts/$file" > tmp_ref 2> tmp_ref_err
    ../build/42sh "scripts/$file" > tmp_def 2> tmp_def_err

    diff tmp_def tmp_ref > res
    diff_content="$(cat res)"
    if [ -n "$diff_content" ]; then
        FAILED="$(($FAILED + 1))"
        pretty_printf_err res
        printf "\n"
    else
        PASSED="$(($PASSED + 1))"
        printf $GREEN"      PASSED\n\n"
    fi

    rm tmp_r*
    rm tmp_d*
    rm res
done

printf $ANNONCE"    ----------------"$DEFAULT"\n"
printf $YELLOW"    RESULTS (GLOBAL)"$DEFAULT"\n"
printf $ANNONCE"    ----------------"$DEFAULT"\n\n"

printf $YELLOW"      TESTED:  $TESTED\n\n"$DEFAULT
printf $GREEN"      PASSED:  $PASSED\n"$DEFAULT
printf $RED"      FAILED:  $FAILED\n\n"$DEFAULT

printf $ANNONCE"-------"$DEFAULT"\n"
printf $YELLOW"  END"$DEFAULT"\n"
printf $ANNONCE"-------"$DEFAULT"\n\n"

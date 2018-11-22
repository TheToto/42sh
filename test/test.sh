#!/bin/sh

GREEN="\033[1;92m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
ANNONCE="\033[38;5;37m"
DEFAULT="\033[1;39m"

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
    "FAILED TEST"*) printf "    "$RED"-------------------\n"
                    printf "    $line\n"
                    printf "    -------------------\n\n"
                    is_err=1;;
    TESTED* ) printf "    "$YELLOW"$line"$DEFAULT"\n\n";;
    PASSED* ) printf "    "$GREEN"$line"$DEFAULT"\n";;
    FAILED* ) printf "    "$RED"$line"$DEFAULT"\n";;
    IGNORED*) printf "    "$DEFAULT"$line"$DEFAULT"\n";;
    *       )
    esac
done < tmp
rm tmp tmp_err

printf "\n"$ANNONCE"  ------------"$DEFAULT"\n"
printf $YELLOW"  GLOBAL TESTS"$DEFAULT"\n"
printf $ANNONCE"  ------------"$DEFAULT"\n\n"

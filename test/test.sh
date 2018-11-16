#!/bin/sh

GREEN="\033[1;92m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
ANNONCE="\033[38;5;37m"
DEFAULT="\033[1;39m"

ceedling > tmp

while read line; do
    case $line in
    [-]*)   printf $ANNONCE"$line"$DEFAULT"\n";;
    PASSED*) printf $GREEN"$line"$DEFAULT"\n";;
    FAILED* | "BUILD FAILURE*") printf $RED"$line"$DEFAULT"\n";;
    Test* | TESTED* | OVERALL*) printf $YELLOW"$line"$DEFAULT"\n";;
    *) echo $DEFAULT"$line"$DEFAULT;;
    esac
done < tmp
rm tmp

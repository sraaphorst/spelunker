#!/bin/bash
printf "\n\n\e[1m\e[40m\e[32m"
printf "***************************\n"
printf "\n\n\e[1m\e[40m\e[32m"
printf "***** EXECUTING TESTS *****\n"
printf "\n\n\e[1m\e[40m\e[32m"
printf "***************************"
printf "\e[0m\n\n"

function printname {
    f=$(basename $1)
    printf "\n\e[1m\e[40m\e[32m"
    printf "***** Executing $f: *****"
    printf "\e[0m\n"
}

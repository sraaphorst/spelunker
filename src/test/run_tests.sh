#!/bin/bash
printf "\n\n\e[1m\e[44m\e[32m***** Executing tests... *****" ; printf "\e[0m\n"

function printname {
    f=$(basename $1)
    printf "\n\e[1m\e[44m\e[32m***** Executing $f... *****" ; printf "\e[0m\n"
}


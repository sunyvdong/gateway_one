#!/bin/bash
if [ 1 -ne $# ]
then
    echo "must input one param, error param is $*"
    exit 1
fi

export RTE_SDK=$(pwd)
cd ${RTE_SDK}/examples/$1
export RTE_TARGET=x86_64-native-linuxapp-gcc
make


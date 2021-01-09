#!/bin/bash

yum install numactl-devel*x86_64
export RTE_SDK=$(pwd)
export RTE_TARGET=x86_64-native-linuxapp-gcc
export RTE_KERNELDIR=${RTE_SDK}/../../linux/linux-4.18.10
make install T=${RTE_TARGET}

#insmod ${RTE_TARGET}/kmod/igb_uio.ko
#insmod ${RTE_TARGET}/kmod/rte_kni.ko
#yum install pciutils -y
#./usertools/dpdk-setup.sh

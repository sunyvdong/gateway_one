#!/bin/bash

main()
{
    #yum install pciutils -y
    #insmod /lib/modules/$(uname -r)/kernel/drivers/uio/uio.ko
    #insmod ../../x86_64-native-linuxapp-gcc/kmod/igb_uio.ko
    #./usertools/dpdk-setup.sh
    ./build/l2fwd -l 2-3 -n 2 -- -q 2 -p 3
}
main

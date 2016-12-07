#!/bin/sh
export PATH=$PATH:/usr/local/popcorn/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/popcorn/lib
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/popcorn/lib
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/popcorn/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/popcorn/include
echo "mounting nfs..."
sudo mount 10.1.1.149:/var/nfs /mnt/nfs/var/nfs
echo "nfs mountdone!"

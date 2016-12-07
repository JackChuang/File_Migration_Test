#!/bin/bash
#spinning untile remote is on
echo "trying..."
while [ "$tmp" != "123" ] 
do
    sleep 1
    tmp=$(ssh 10.1.1.105 echo "123")
done
echo ""
sleep 3
#echo ""
#echo ""
#echo ""
#tmp1=$(lsmod | grep "msg_layer ")
#tmp2=$(ssh 10.1.1.105 lsmod |grep "msg_layer ")
#if [ "$tmp1" != "" ] 
#then
#    echo "local: $tmp1"
#    exit
#fi
#if [ "$tmp2" != "" ] 
#then
#    echo "remote: $tmp2"
#    exit
#fi

tmp1=$(lsmod | grep "msg_layer " |wc -l)
tmp2=$(ssh 10.1.1.105 lsmod |grep "msg_layer "| wc -l)
if [ "$tmp1" -gt 0 ] 
then
    echo "local: $tmp1"
    exit
fi
if [ "$tmp2" -gt 0 ] 
then
    echo "remote: $tmp2"
    exit
fi
#echo ""
#echo ""
#echo ""
#echo "if see only 1 above, REBOOT!!!!!!!!!"
echo "-> Remote is alive."
sleep 1

#echo "checking remote is alive..."
#tmp=$(ssh 10.1.1.105 echo "123")
#if [ "$tmp" != "123" ] 
#then
#    exit
#else
#    echo "-> Remote is alive."
#fi

echo -n "installing msg layer"
ssh 10.1.1.105 sudo insmod msg_layer.ko&
sleep 3
echo -n "..."
sleep 3
echo -n "..."
sleep 4
echo -n "..."
sleep 4
echo ""
sleep 2
sudo insmod msg_layer.ko
echo "msg layer up"



cd file3
source env.sh
rm file
make clean
make
cd ..
ssh 10.1.1.105 rm -r file3
scp -r file3 10.1.1.105:~


sleep 0.5

tmp="file_x86-64"
ln file3/$tmp file3/file
ssh 10.1.1.105 ln file3/file_aarch64 file3/file

sudo dmesg -c > /dev/null
ssh 10.1.1.105 sudo dmesg -c > /dev/null



#clear NFS debug files
rm /mnt/nfs/var/nfs/arm_printout.txt
rm /mnt/nfs/var/nfs/example.txt
rm /mnt/nfs/var/nfs/arm_sys_out


sleep 0.5

echo "done"
echo "X86"
file file3/file
lsmod |grep "msg_layer "

echo "ARM"
ssh 10.1.1.105 file file3/file
ssh 10.1.1.105 lsmod |grep "msg_layer "


cd mklinux-utils/ns
sudo ./launch_ns.sh

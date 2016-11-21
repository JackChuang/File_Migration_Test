#!/bin/bash
#spinning untile remote is on
echo "got $# arguments"

FILE="file4MultipleMigration"

echo "trying my peer 10.1.1.105..."
while [ "$tmp" != "123" ] 
do
    sleep 1
    tmp=$(ssh 10.1.1.105 echo "123")
done
echo ""
echo ""
echo ""
sleep 5
if [ ! -z "$1" ] # not null
then
    #a=$1
    for ((i=0 ;i<=$1 ;i++))
    do
        ssh 10.1.1.105 echo "remote is alive"
        sleep 1
    done
    #for i in {0.."$a"..1}
    #do 
    #done
fi
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
    echo "local: has $tmp1 msg_layer.ko"
    exit
fi
if [ "$tmp2" -gt 0 ] 
then
    echo "remote: has $tmp2 msg_layer.ko"
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

for i in {0..15..1}
do 
    sleep 1
    echo -n "."                                                                                           
done
echo ""
sleep 2
sudo insmod msg_layer.ko
echo "msg layer up"



cd $FILE
source env.sh
rm file
make clean
make
cd ..
ssh 10.1.1.105 rm -r $FILE
scp -r $FILE 10.1.1.105:~


sleep 0.5

tmp="file_x86-64"
ln $FILE/$tmp $FILE/file
ssh 10.1.1.105 ln $FILE/file_aarch64 $FILE/file

sudo dmesg -c > /dev/null
ssh 10.1.1.105 sudo dmesg -c > /dev/null



#clear NFS debug files
rm /mnt/nfs/var/nfs/arm_printout.txt
rm /mnt/nfs/var/nfs/example.txt
rm /mnt/nfs/var/nfs/arm_sys_out


sleep 0.5

echo "done"
echo "X86"
file $FILE/file
lsmod |grep "msg_layer "

echo "ARM"
ssh 10.1.1.105 file $FILE/file
ssh 10.1.1.105 lsmod |grep "msg_layer "


cd /home/jackchuang/mklinux-utils/ns
sudo ./launch_ns.sh

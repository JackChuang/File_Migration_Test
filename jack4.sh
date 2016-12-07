#!/bin/bash
#spinning untile remote is on
FILE="file4MultipleMigration" #file2
ABS_FILE_PRE=`pwd`
ABS_FILE="$ABS_FILE_PRE"/"$FILE"

remote_ip="10.1.1.105"
echo "got $# arguments"

echo "trying..."
while [ "$tmp" != "123" ] 
do
    sleep 1
    tmp=$(ssh $remote_ip echo "123")
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
        ssh $remote_ip echo "remote is alive"
        sleep 1
    done
    #for i in {0.."$a"..1}
    #do 
    #done
fi


tmp1=$(lsmod | grep "msg_layer " |wc -l)
tmp2=$(ssh $remote_ip lsmod |grep "msg_layer "| wc -l)
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
echo "->->-> Remote is alive."
sleep 1


echo -n "installing msg layer"
ssh $remote_ip sudo insmod msg_layer.ko&

for i in {0..15..1}
do 
    sleep 1
    echo -n "."
done
echo ""
sleep 2
sudo insmod msg_layer.ko
echo "msg layer up"



cd $ABS_FILE
source env.sh
rm file
make clean
make
cd ..
ssh $remote_ip rm -r $ABS_FILE
ssh $remote_ip mkdir -p $ABS_FILE_PRE ####
scp -r $ABS_FILE $remote_ip:$ABS_FILE_PRE ####


sleep 0.5

tmp="file_x86-64"
ln $ABS_FILE/$tmp $ABS_FILE/file
ssh $remote_ip ln $ABS_FILE/file_aarch64 $ABS_FILE/file

sudo dmesg -c > /dev/null
ssh $remote_ip sudo dmesg -c > /dev/null



#clear NFS debug files
rm /mnt/nfs/var/nfs/arm_printout.txt
rm /mnt/nfs/var/nfs/example.txt
rm /mnt/nfs/var/nfs/arm_sys_out


sleep 0.5

echo "done"
echo "X86"
file $ABS_FILE/file
lsmod |grep "msg_layer "

echo "ARM"
ssh $remote_ip file $ABS_FILE/file
ssh $remote_ip lsmod |grep "msg_layer "


cd /home/jackchuang/mklinux-utils/ns
sudo ./launch_ns.sh

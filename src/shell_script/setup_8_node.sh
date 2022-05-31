#!/bin/bash


echo "**** Generating rsa keys ****"
su -c "ssh-keygen -t rsa -f /home/cpd2021/.ssh/id_rsa -q -P ''" -m cpd2021

echo "**** STARTING SSH SERVICE ****"

service ssh start
service ssh status

echo "**** Adding IPs to Host file ****"

echo "${1} master" |  tee -a /etc/hosts
echo "${2} node01" |  tee -a /etc/hosts
echo "${3} node02" |  tee -a /etc/hosts
echo "${4} node03" |  tee -a /etc/hosts
echo "${5} node04" |  tee -a /etc/hosts
echo "${6} node05" |  tee -a /etc/hosts
echo "${7} node06" |  tee -a /etc/hosts
echo "${8} node07" |  tee -a /etc/hosts

exit
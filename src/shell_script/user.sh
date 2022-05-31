#!/bin/bash

echo "**** Making new user ****"
adduser --quiet --disabled-password --shell /bin/bash --home /home/cpd2021 --gecos "cpd2021" cpd2021

echo "cpd2021" > password.txt
echo "cpd2021:cpd2021" | chpasswd

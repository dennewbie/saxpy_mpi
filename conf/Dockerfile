FROM ubuntu:18.04

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8


RUN apt-get --yes -qq update \
 && apt-get --yes -qq upgrade \
 && apt-get --yes -qq install \
                      bzip2 \
                      cmake \
                      cpio \
                      curl \
                      g++ \
                      gcc \
                      gfortran \
                      git \
                      gosu \
                      libblas-dev \
                      liblapack-dev \
                      libopenmpi-dev \
                      openmpi-bin \
                      python3-dev \
                      python3-pip \
                      virtualenv \
                      wget \
                      zlib1g-dev \
                      vim       \
                      htop      \
                      net-tools \
                      openssh-server \
                      nfs-server  \
                      nfs-client  \
                      iputils-ping \
                      kmod \
 && apt-get --yes -qq clean \
 && rm -rf /var/lib/apt/lists/*


CMD [ "/bin/bash" ]

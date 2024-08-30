FROM ubuntu:22.04

ARG UID=0
ARG GID=0

RUN apt-get update && apt-get install wget lsb-release software-properties-common -y && apt-get clean all
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
RUN apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
RUN apt update && apt install kitware-archive-keyring && rm /etc/apt/trusted.gpg.d/kitware.gpg

COPY packages.txt .
RUN apt-get update && DEBIAN_FRONTEND=noninteractive xargs -a packages.txt apt-get install -y
RUN ln -s /usr/bin/python3 /usr/bin/python
RUN ln -s /usr/bin/clang-15 /usr/bin/clang
RUN ln -s /usr/bin/clang /usr/bin/clang++
RUN ln -s /usr/bin/clang /usr/bin/clang-cpp
RUN ln -s /usr/bin/clang /usr/bin/clang-cl
ENV CXX=/usr/bin/clang++

RUN groupadd --gid ${GID} buildergroup
RUN useradd --uid ${UID} --gid ${GID} --create-home builder
USER builder
RUN mkdir -p /home/builder/build/
WORKDIR /home/builder/build/

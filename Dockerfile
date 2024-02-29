FROM ubuntu:22.04

COPY packages.txt .
RUN apt-get update && DEBIAN_FRONTEND=noninteractive xargs -a packages.txt apt-get install -y
RUN mkdir -p /root/build/
RUN ln -s /usr/bin/python3 /usr/bin/python
WORKDIR /root/build/
ENV CXX=/usr/bin/clang++-15

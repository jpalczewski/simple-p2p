FROM alpine

MAINTAINER Jacek Palczewski <jpalczewski@gmail.com>

RUN apk add --update boost-filesystem boost-dev openssl-dev cmake g++ make tcpdump \
    && mkdir -p /simple-p2p/build

COPY . /simple-p2p

RUN cd /simple-p2p/build \
    && cmake .. \
    && make \
    && make install \
    && mkdir /simple-p2p/release/files

ENTRYPOINT /simple-p2p/release/simple-p2p-daemon

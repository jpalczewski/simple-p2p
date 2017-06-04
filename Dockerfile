FROM alpine

# maybe add ARGS for server port, client port..

MAINTAINER Jacek Palczewski <jpalczewski@gmail.com>

#small helpers
RUN apk add --update tmux htop

RUN apk add --update boost-filesystem boost-dev openssl-dev cmake g++ make tcpdump \
    && mkdir -p /simple-p2p/build

COPY . /simple-p2p

RUN cd /simple-p2p/build \
    && cmake .. \
    && make \
    && make install \
    && mkdir /simple-p2p/release/files

ENTRYPOINT /simple-p2p/release/simple-p2p-daemon 5000 5001

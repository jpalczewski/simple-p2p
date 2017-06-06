FROM alpine

# maybe add ARGS for server port, client port..

MAINTAINER Jacek Palczewski <jpalczewski@gmail.com>

# small helpers
RUN apk add --update tmux htop vim

RUN apk add --update boost-filesystem boost-dev openssl-dev cmake g++ make tcpdump \
    && mkdir -p /simple-p2p/build
	
COPY . /simple-p2p

# if we would like to dinamicaly build configi.ini this would return broadcast ip
# RUN echo $(ifconfig eth0 | grep -o 'inet addr:[0-9]\{1,3\}.\{1,3\}' | cut -d: -f2)".255.255"

RUN cd /simple-p2p/build \
    && cmake .. \
    && make \
    && make install \
    && ln -s /simple-p2p/release/simple-p2p-cli /usr/bin/simple-p2p-cli 

ENTRYPOINT /simple-p2p/release/simple-p2p-daemon /simple-p2p/profiles/docker/

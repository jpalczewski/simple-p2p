# simple-p2p
Simple P2P protocol client. 

## Build instructions
```
$ git clone ${REPO_URL}
$ cd simple-p2p
$ mkdir build
$ cd build
$ cmake ..
$ make install 
```
## Usage
### Without docker
```
$ simple-p2p-daemon ${PROFILE_DIRECTORY}
And for example:
$ simple-p2p-cli --port ${PORT_SPECIFIED_IN_PROFILE} --display
```
###With docker
In repo main directory:
```
$ docker-compose up
```
# CHANGELOG:


- config.json
- package.json

for homebridge running in a docker container on rapsberry pi 1

# Changes
20171117:
- raspberry currently running in Podest Hemmerde
- added dyson 360 packae and config
{
  "accessory": "Dyson360EyeRobotVacuum",
  "name": "Cleano",
  "host": "192.168.1.48",
  "port": 1883,
  "username": "JJ5-UK-HEA1285A",
  "password": "o6IJWuk9C+HcX1w2zVi3TJC3NDD2Bx6atpEBLFCo087wzQGJEwPHJOwqcKj5fDv0rYA0sr4ZRgszo84ZBK5v7g==",
  "refresh": 0
},


Password of dyson has to be first converted to SHA-12 and then to Base64. Carried out via

https://caligatio.github.io/jsSHA/

This can be done via a tool at https://caligatio.github.io/jsSHA/ (client side javascript only). Put password into 'Input text', input type 'TEXT', SHA Variant 'SHA-512', Number of Rounds "1", Output Type "Base64":

kkszw87D+XQHg3nLvS4PiWnlLdGYHd3wRfXOkO6r0ksT3sQ6/cMhvO3CachwZDPk+rFs+qTmSZBxpmLoaJiZPQ==



{
  "accessory": "Dyson360EyeRobotVacuum",
  "name": "Molli",
  "host": "192.168.1.76",
  "port": 1883,
  "username": "JG8-EU-HKA2172A",
  "password": "kkszw87D+XQHg3nLvS4PiWnlLdGYHd3wRfXOkO6r0ksT3sQ6/cMhvO3CachwZDPk+rFs+qTmSZBxpmLoaJiZPQ==",
  "refresh": 0
}

20171117:
Currently cannot connect to Molli ( no wifi, Molli does not connect to Dyson app either)


# Troubleshooting
If the home app says:"Device already added, you need to reset it first ..", then delete content in the folders residing in the project folder 'homebridge':
- accessories (cache file)
- persist


# Installing OPENHAB 2.0 as docker container

Prerequesites:

- install docker
- install docker-compose
- You need to export the variable for it to be available in the childprocess
  export DOCKER_HOST=127.0.0.1

## Create openhab user
Just because one is running in an isolated container does not mean running as
root is recommended. So first create an ```openhab``` user configured to be a system
user with no home and no shell. This can be done on Ubunutu and Raspbian with the command:

    sudo useradd -r -S /sbin/nologin openhab
Add your regular user to the openhab group.

    usermod -a -G openhab <user>

Add your regular user to the docker group:

    usermod -aG docker ${USER}

AND DO NOT FORGET TO REGLIN AFTER THAT.

Test with

        groups

if user was added to the specified group.

## Pull openhub docker container for arm7 devices:

    cd openhab
    sudo docker pull openhab/openhab:2.1.0-armhf-alpine

## Create the openHAB conf, userdata, and addon directories

These directories will be mounted into the running docker container and are where configurations and persistence data will be stored. Note, that software running inside a Docker container cannot follow the symbolic links located ina mounted volume. Make sure the ```openhab ```user owns these directories.

    mkdir ./openhab
    cd openhab
    mkdir ./openhab/conf
    mkdir ./openhab/userdata
    mkdir ./openhab/addons
    chown -R openhab:openhab

The upper folders reside in the created ``` openhab``` folder.

### Create docker-compose.yml inside project folder
**AVOID USING TABS INSIDE THE YAML FILE!**

docker-compose.yml: (see file in the same folder as this file)
```

```

```--net=host``` : by default Docker will place a container into its own network stack. However, openHAB 2 requires UPnP discovery so this parameter makes the Docker container use the host’s network stack.


## Running the Container as a Service Managed by Docker
Inside the project folder run:
    docker-compose run -p 8080:8080 -p 8443:8443 openhab

### Troubelshooting

Even inside container everything works fine(I checked by executing curl request inside container):

    docker exec -it <container> bash -c 'curl http://localhost:8080'
Check comtainer binding ports to the host via:

    sudo iptables -t nat -L -n

Or

    sudo netstat -lt|grep 8080

Note: by default Docker containers can make connections to the outside world, but the outside world cannot connect to containers.

```
    # What your NAT rules might look like when Docker
    # is finished setting up a -P forward:

    $ iptables -t nat -L -n

    ...
    Chain DOCKER (2 references)
    target     prot opt source               destination
    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:49153 to:172.17.0.2:80

    # What your NAT rules might look like when Docker
    # is finished setting up a -p 80:80 forward:

    Chain DOCKER (2 references)
    target     prot opt source               destination
    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:80 to:172.17.0.2:80
```
Error:

    Cannot create container for service openhab: conflicting options: host type networking can't be used with links. This would result in undefined behavior

    With this docker-compose.yml, the error will happen when the backend container is already running and we attempt to execute another docker run command related to backend.

    Here's a concrete example. Assuming that backend is already running, this is the behavior:

    # When running `frontend` container, there's no problem
    docker-compose run frontend cp -rf /code/static_files/ /code/frontend_assets/
    # But once you run the `backend` container while there's already one running...
    docker-compose run backend python manage.py collectstatic --noinput
    You will immediately see this error:

    ERROR: Cannot create container for service backend: Conflicting options: host type networking can't be used with links. This would result in undefined behavior
    Workaround

    There's a workaround for this issue that works for me, which is to do a docker-compose down prior to doing docker-compose run backend. Eg, this is what I do now:

    docker-compose down
    docker-compose run frontend cp -rf /code/static_files/ /code/frontend_assets/
    # No problem. We are happy! ^_^
    docker-compose run backend python manage.py collectstatic --noinput


    ---


When you use --net=host you do not need to use EXPOSE nor do you need to use -p to expose the ports from the container. This option makes the container use the host's network stack rather than a docker container network stack so all servers that bind to a port take that port from the host and appear on the network that way.

If you are not using --net=host, you need to publish the needed ports using the -p option:

-p 8080:8080 -p 8101:8101
An article I recently read says that the EXPOSE option in a Dockerfile should be treated as documentation. You still need to explicitly publish the ports or else Docker assigns the exposed ports to random host ports.

### Possible errors

```
+ set -euo pipefail
+ IFS='
	'
+ NEW_USER_ID=9001
+ echo 'Starting with openhab user id: 9001'
+ id -u openhab
Starting with openhab user id: 9001
+ echo 'Create user openhab with id 9001'
+ adduser -u 9001 --disabled-password --gecos '' --home /openhab openhab
Create user openhab with id 9001
adduser: Only root may add a user or group to the system.
+ case ${OPENHAB_VERSION} in
++ ls -A /openhab/userdata
+ '[' -z 'etc
logs
tmp' ']'
++ ls -A /openhab/conf
+ '[' -z 'html
icons
items
persistence
rules
scripts
services
sitemaps
sounds
things
transform' ']'
+ chown -R openhab:openhab /openhab
chown: invalid user: ‘openhab:openhab’
```

That behaviour is correct. You are starting the container with --user=999 (as non root). The entryscript can not add the user openhab, because you are starting as non-root and it fails with invalid user. Removing the --user should do the job.


### Alternative setup:

Following up to help anyone else looking for the Compose formatting. I finally had a kind soul point out what I was missing in the formatting on Reddit, and was able to successfully get this network config into my Compose file. Formatting below as a sample - this will allow setting up a macvlan based network in your docker host, along with the ability to specify a manual IP address for the system - this will let you have less problems with your UPnP based integrations and potential port conflicts on said services.

networks:
  mynet:
    driver: macvlan
    driver_opts:
      parent: "ens160"
    ipam:
      config:
      - subnet: 10.0.10.0/24
        gateway: 10.0.10.1
        ip_range: 10.0.10.224/28
You can then use the following in the individual Compose service section:

      networks:
        mynet:
          ipv4_address: 10.0.10.200

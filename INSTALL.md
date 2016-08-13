# INSTALLATION GUIDE FOR NEURAL-WEATHER-FORECAST-OPTIMIZATION

For installing this project do :

1. needs to install dependencies (see [INSTALLATION OF DEPENDENCIES](#DEPENDENCIES))
2. clone git repo (see [CLONE GIT REPOSITORY](#CLONE))
3. install QT-Creator (see [INSTALL QT-CREATOR](#QT))
4. compile project (see [COMPILE PROJECT](#COMPILE))
5. configure project (see [CONFIGURE PROJECT](#CONFIGURE))

If you would like to have some graphical user interface to plot the time series stored in InfluxDB install Grafana.

1. install Grafana (see [INSTALL GRAFANA](#INSTALL_GRAFANA))
2. configure Grafana (see [CONFIGURE GRAFANA](#CONFIGURE_GRAFANA))

## <a name="DEPENDENCIES"></a> INSTALLATION OF DEPENDENCIES

###install influxDB

go to https://influxdata.com/downloads/#influxdb and download or do : 

for Ubuntu & Debian (64-bit)
`wget https://dl.influxdata.com/influxdb/releases/influxdb_0.13.0_amd64.deb`
`sudo dpkg -i influxdb_0.13.0_amd64.deb `

for Ubuntu & Debian (ARM)
`wget https://dl.influxdata.com/influxdb/releases/influxdb_0.13.0_armhf.deb`
`sudo dpkg -i influxdb_0.13.0_armhf.deb`

then start the influxDB-daemon :
`sudo systemctl start influxdb`


###install curl

`sudo apt-get install curl`

## <a name="CLONE"></a> CLONE GIT REPOSITORY
todo
## <a name="QT"></a> INSTALL QT-CREATOR
todo
## <a name="COMPILE"></a> COMPILE PROJECT
todo
## <a name="CONFIGURE"></a> CONFIGURE PROJECT
todo

## FOR GRAPHICAL PLOTS OF THE TIME-SERIES
If you want to use a webinterface for plotting the time-series you might like Grafana

## <a name="INSTALL_GRAFANA"></a> INSTALL GRAFANA
go to http://grafana.org/download/ or

###for normal AMD64-architecture

`wget https://grafanarel.s3.amazonaws.com/builds/grafana_3.1.1-1470047149_amd64.deb`
`sudo apt-get install -y adduser libfontconfig`
`sudo dpkg -i grafana_3.1.1-1470047149_amd64.deb`

###for arm-architecture (e.g. on Raspberry Pi)

follow either http://giatro.me/2015/09/30/install-influxdb-and-grafana-on-raspberry-pi.html or do

#### install GO

```
# requirements
sudo apt-get install curl git mercurial make binutils bison gcc build-essential
# install gvm
bash < <(curl -s -S -L https://raw.githubusercontent.com/moovweb/gvm/master/binscripts/gvm-installer)
source /home/pi/.gvm/scripts/gvm
# install go
gvm install go1.4 
gvm use go1.4 
export GOROOT_BOOTSTRAP=$GOROOT 
gvm install go1.5
gvm use go1.5 --default
```

#### install NodeJS
```
# add the apt.adafruit.com package repository to your Pi
curl -sLS https://apt.adafruit.com/add | sudo bash
# install node.js using apt-get
sudo apt-get install node
```
#### install Grafana

```
# getting the source
go get github.com/grafana/grafana
# building the backend
cd $GOPATH/src/github.com/grafana/grafana
go run build.go setup
$GOPATH/bin/godep restore
go run build.go build
# build the front-end assets
npm install
sudo npm install -g grunt-cli
grunt --force
# start grafana
$GOPATH/bin/grafana
```



## <a name="CONFIGURE_GRAFANA"></a> CONFIGURE GRAFANA
### start Grafana
```
# start grafana
cd $GOPATH/src/github.com/grafana/grafana
$GOPATH/bin/grafana`
```
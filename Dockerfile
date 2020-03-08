# FROM arduino/arduino-cli:builder-1

# RUN arduino-cli core update-index
# RUN arduino-cli core install arduino:avr

# WORKDIR /work

# FROM golang:1.11-stretch

# RUN apt update -y
# RUN apt install git make -y
# RUN useradd -m arduino

# RUN go get -u github.com/arduino/arduino-cli
# RUN arduino-cli core update-index
# RUN arduino-cli core install arduino:samd

# ENTRYPOINT ["arduino-cli"]
# CMD []

FROM linuxbrew/linuxbrew

RUN brew update
RUN brew install arduino-cli

RUN arduino-cli core update-index
RUN arduino-cli core install arduino:avr

WORKDIR /work

RUN arduino-cli lib install "DHT sensor library"
RUN arduino-cli lib install "RTClib"

#ENTRYPOINT ["arduino-cli compile --fqbn arduino:avr:nano /work/sklenik.ino"]



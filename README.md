# LKM-Get-Packet
Get the 5 tuple of the packet with the specified source IP from the kernel.
## Features

Get the 5-tuple of the packet specifying the source IP from the kernel, source address, destination address, original port, destination port, and protocol, and pass the relevant information to the application, which stores the information in a file.

## Usage

1. git clone
2. `cd LKM-Get-Packet/`
3. `sudo ./build.sh`
4. `cd user/`
5. `go run user.go xxx.xxx.xxx.xxx`

......

`sudo rmmod get_packet`

## Usage Examples

![image](https://user-images.githubusercontent.com/51999056/96440648-dbcd4c80-123a-11eb-8cb6-b00f5c50b2d2.png)

## Dev environment

kernel: `4.4.0-193-generic`

gcc: `5.4.0`
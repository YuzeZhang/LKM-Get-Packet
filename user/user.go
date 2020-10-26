package main

import (
	"fmt"
	"os"
	"syscall"
	"time"
	"net"
)

func main () {
	ipStr := os.Args[1]
	// Checking IP address validity
	if net.ParseIP(ipStr) == nil {
		fmt.Println("The ip address is not in the correct format, please enter the correct ip address (ipv4)!")
		return
	}

	ipByte := []byte(ipStr)

	// open dev
	fd, err := syscall.Open("../mychardev0", syscall.O_RDWR, 0777)
	if err != nil {
		println("Open dev error!\n")
		return
	}

	// send ip addr to lkm (by shm)
	_, err = syscall.Write(fd, ipByte)
	if err != nil {
		println("Send ip addr to lkm error!\n")
		return
	}

	buf := make([]byte, 4096)
	time.Sleep(1 * time.Second)

	fp, err := os.OpenFile("output.txt", os.O_CREATE | os.O_APPEND | os.O_WRONLY, 0777)

	for {
		for {
			// get 5-tuple from lkm (by shm)
			i, err := syscall.Read(fd, buf)
			if err != nil {
				println("Get 5-tuple from lkm error!\n")
				return
			}
			if i <= 0 || i > 4096{
				break
			}
			writeBuf := buf[:i]
			fmt.Printf("%s\n", string(writeBuf))
			_, err = fp.Write(writeBuf)
			if err != nil {
				println("Write to file error!\n")
				return
			}
			fp.Seek(0, 2)
		}
		time.Sleep(1 * time.Second)
	}
}
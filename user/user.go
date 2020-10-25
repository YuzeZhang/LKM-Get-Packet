/*
package main

// #include "user.h"
import "C"

import (
	"os"
	"unsafe"
)

func main() {
	ipStr := os.Args[1]
	cIpStr := C.CString(ipStr)
	buf := C.get_c_buf()

	if C.check_input_ip(cIpStr) == -1 {
		println("The input ip address format is not correct, it should be xxx.xxx.xxx.xxx\n")
		return
	}

	// open dev
	fd := C.open_dev_file()
	if fd < 0 {
		println("Open dev error!\n")
		return
	}

	// send ip addr to lkm (by shm)
	if C.my_write(fd, cIpStr) == -1 {
		println("Send ip addr to lkm error!\n")
		return
	}

	// get 5-tuple from lkm (by shm)
	if C.my_read(fd, buf) == -1 {
		println("Get 5-tuple from lkm error!\n")
		return
	}

	C.c_print(buf)

	C.free(unsafe.Pointer(buf))
}

package main

import (
	. "fmt"
	"runtime"
)

var i int = 0


func server(){
	var globalVar int = 0
	var i1, i2 int
	for{
		select{
			case i1 := <- c1:
				globalVar++
				print("received ", i1, " from c1\n")
			case i2 := <- c2:
				globalVar--
				print("received ", i2, " from c2\n")

			case <- c3:
				done1 <- true
			case <- c4:
				done2 <- true
		}
	}
	select{}
}

func foo1() {
	for j := 0 ; j <1000000; j++{

		c1 <- 1
	}
	c3 <- 1
	return
}

func foo2() {
	for j := 0 ; j <1000000; j++{
		c2 <- 1	
	}
	c3 <- 1
	return 
}

func main() {

	runtime.GOMAXPROCS(runtime.NumCPU())    // I guess this is a hint to what GOMAXPROCS does...
                                                // Try doing the exercise both with and without it!
	var c1, c2, c3, c4 chan int
	var done1, done2 chan bool
	go server()
	go foo1()                               // This spawns someGoroutine() as a goroutine
	go foo2()
	
    // We have no way to wait for the completion of a goroutine (without additional syncronization of some sort)
    // We'll come back to using channels in Exercise 2. For now: Sleep.
	<-done1
	<-done2
	Println("Hello from main!")
	Println(i)
}

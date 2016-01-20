package main

import (
	. "fmt"
	"runtime"
)

var i int = 0



func foo1(mutex_channel chan int, foo1_channel chan int) {
	for j := 0 ; j <1000000; j++{
		mutex_channel <- 1
		i++
		<- mutex_channel
	}
	foo1_channel <- 1
	return 
}

func foo2(mutex_channel chan int, foo2_channel chan int) {
	for j := 0 ; j <1000000; j++{
		mutex_channel <- 1
		i--
		<- mutex_channel
	}
	foo2_channel <-1
	return 
}

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())    // I guess this is a hint to what GOMAXPROCS does...
    mutex_channel := make(chan int, 1)
    foo1_channel := make(chan int, 1)
    foo2_channel := make(chan int, 1)





                                                // Try doing the exercise both with and without it!
	go foo1(mutex_channel, foo1_channel)                               // This spawns someGoroutine() as a goroutine
	go foo2(mutex_channel, foo2_channel)
	
    // We have no way to wait for the completion of a goroutine (without additional syncronization of some sort)
    // We'll come back to using channels in Exercise 2. For now: Sleep.

	<- foo1_channel
	<- foo2_channel
	Println("Hello from main!")
	Println(i)
}

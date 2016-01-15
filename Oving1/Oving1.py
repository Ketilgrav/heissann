from threading import Thread

i = 0
# Potentially useful thing:
#   In Python you "import" a global variable, instead of "export"ing it when you declare it
#   (This is probably an effort to make you feel bad about typing the word "global")

def foo1():
	global i
	for j in xrange(0, 1000000):
		i += 1
	return 0

def foo2():
	global i
	for j in xrange(0, 1000000):
		i -= 1
	return 0



def main():
	first = Thread(target = foo1, args = (),)
	first.start()

	second = Thread(target = foo2, args = (),)
	second.start()
	
	first.join()
	second.join()
	print("Hello from main!")
	print(i)


main()

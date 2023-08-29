CC=gcc
CFLAGS=-lwiringPi
powerApp=powerUsage
lib=spiADE9078

power: $(powerApp).o $(lib).o
	$(CC) $(powerApp).o $(lib).o -o ./build/$(powerApp) $(CFLAGS)
	./build/$(powerApp) /dev/spidev0.0

$(powerApp).o: $(powerApp).c
	$(CC) -c $(powerApp).c

$(lib).o: $(lib).c $(lib).h
	$(CC) -c $(lib).c



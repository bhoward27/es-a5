CC = arm-linux-gnueabihf-gcc
CFLAGS = -D _POSIX_C_SOURCE=200809L -pthread -g -std=gnu11 -Wall -Werror
SOURCES = utils.c log.c gpio.c shutdown.c accelerometer.c i2c.c main.c
OUT_DIR = $(HOME)/cmpt433/public/myApps

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUT_DIR)/find-dot

clean:
	rm $(OUT_DIR)/find-dot


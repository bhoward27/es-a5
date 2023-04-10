CC = arm-linux-gnueabihf-gcc
CFLAGS = -g -std=gnu11 -Wall -Werror
LFLAGS = -D _POSIX_C_SOURCE=200809L -pthread -lm
SOURCES = utils.c log.c gpio.c shutdown.c accelerometer.c i2c.c main.c
OUT_DIR = $(HOME)/cmpt433/public/myApps

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LFLAGS) -o $(OUT_DIR)/find-dot

clean:
	rm $(OUT_DIR)/find-dot


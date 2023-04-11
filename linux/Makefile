OUT_DIR = $(HOME)/cmpt433/public/myApps
LOG_LEVEL_FILE=$(OUT_DIR)/logLevel.txt

CC = arm-linux-gnueabihf-gcc
CFLAGS = -g -std=gnu11 -Wall -Werror
LFLAGS = -D _POSIX_C_SOURCE=200809L -pthread -lm
SOURCES = utils.c log.c gpio.c shutdown.c target.c accelerometer.c i2c.c digit_display.c joystick.c buzzer.c main.c 

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LFLAGS) -o $(OUT_DIR)/find-dot

# Makes new log level file if it doesn't exist.
	./makeLogLevelFile.sh $(LOG_LEVEL_FILE)
clean:
	rm $(OUT_DIR)/find-dot


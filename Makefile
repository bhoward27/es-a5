# RUN THIS ON THE HOST!

ifneq ("$(wildcard /ID.txt )", "")
# Target:
# If you run it on the target, it will print the following error.
all:
	@echo
	@echo "ERROR: Don't run *this* makefile on the BBG"
	@echo "   This makefile copies files to the shared folder"
	@echo "   It looks like you are running it on your BBG"
	@echo "   Instead, change into the specific sub-directory for your"
	@echo "   current project and run 'make' there."

else
# Host:
# Cross-compile C/C++ code, and copy PRU code to target
all: nested-cross-compile pru-copy

# Cross compile your C/C++ programs
# Uncomment this for any folder you want to cross-compile
nested-cross-compile:
	@make --directory=linux

pru-copy:
	mkdir -p $(HOME)/cmpt433/public/pru/
	cp -r * $(HOME)/cmpt433/public/pru/
	@echo "COPY ONLY > $(HOME)/cmpt433/public/pru/_COPY_ONLY_"
	@echo ""
	@echo "You must build the PRU code on the target, then install it:"
	@echo "(bbg)$$ cd /mnt/remote/pru/pru/"
	@echo "(bbg)$$ make"
	@echo "(bbg)$$ sudo make install_PRU0"
endif
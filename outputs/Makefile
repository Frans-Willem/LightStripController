all: serial

serial: force_look
	cd $@; make $(MFLAGS)
clean: force_look
	cd serial; make clean $(MFLAGS)
force_look:
	true
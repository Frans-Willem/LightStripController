all: serial ftdi

serial: force_look
	cd $@; make $(MFLAGS)
ftdi: force_look
	cd $@; make $(MFLAGS)
clean: force_look
	cd serial; make clean $(MFLAGS)
	cd ftdi; make clean $(MFLAGS)
force_look:
	true
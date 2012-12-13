all: serial

serial: force_look
	cd $<; make $(MFLAGS)
force_look:
	true
all: sinfade sinsweep

sinfade: force_look
	cd $@; make $(MFLAGS)
sinsweep: force_look
	cd $@; make $(MFLAGS)
clean: force_look
	cd sinfade; make clean $(MFLAGS)
	cd sinsweep; make clean $(MFLAGS)
force_look:
	true
all: common core loader outputs

common: force_look
	cd common; make $(MFLAGS)
core: force_look
	cd core; make $(MFLAGS)
loader: force_look
	cd loader; make $(MFLAGS)
outputs: force_look
	cd outputs; make $(MFLAGS)
clean: force_look
	cd common; make clean $(MFLAGS)
	cd core; make clean $(MFLAGS)
	cd loader; make clean $(MFLAGS)
	cd outputs; make clean $(MFLAGS)
	rm -rf bin
	
force_look:
	true
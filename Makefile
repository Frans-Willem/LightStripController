all: common core loader outputs generators transitions

libjson: force_look
	cd $@; SHARED=1 make $(MFLAGS)
common: force_look
	cd $@; make $(MFLAGS)
core: force_look libjson
	cd $@; make $(MFLAGS)
loader: force_look
	cd $@; make $(MFLAGS)
outputs: force_look
	cd $@; make $(MFLAGS)
generators: force_look
	cd $@; make $(MFLAGS)
transitions: force_look
	cd $@; make $(MFLAGS)
clean: force_look
	cd libjson; SHARED=1 make clean $(MFLAGS)
	cd common; make clean $(MFLAGS)
	cd core; make clean $(MFLAGS)
	cd loader; make clean $(MFLAGS)
	cd outputs; make clean $(MFLAGS)
	cd generators; make clean $(MFLAGS)
	cd transitions; make clean $(MFLAGS)
	rm -rf bin
	
force_look:
	true
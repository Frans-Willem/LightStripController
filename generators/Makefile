all: rainbow color knightrider

rainbow: force_look
	cd $@; make $(MFLAGS)
color: force_look
	cd $@; make $(MFLAGS)
knightrider: force_look
	cd $@; make $(MFLAGS)
clean: force_look
	cd rainbow; make clean $(MFLAGS)
	cd color; make clean $(MFLAGS)
	cd knightrider; make clean $(MFLAGS)
force_look:
	true
all: bin/common.so bin/core.so bin/loader

bin/common.so: force_look
	cd common; make $(MFLAGS)
bin/core.so: force_look
	cd core; make $(MFLAGS)
bin/loader: force_look
	cd loader; make $(MFLAGS)
force_look:
	true
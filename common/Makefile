CC=g++
CPPFLAGS=-I./include -fPIC
DEPS=
OBJDIR=obj
SRCDIR=src
DEPDIR=depends
SRCS=CTime.cpp CColor.cpp IOutput.cpp
OBJS=$(patsubst %, $(OBJDIR)/%,$(SRCS:.cpp=.o))
DEPS=$(patsubst %, $(DEPDIR)/%,$(SRCS:.cpp=.d))
TARGETPATH=../bin
TARGET=$(TARGETPATH)/common.so

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(TARGETPATH)
	$(CC) -shared -o $@ $^ -lrt

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CPPFLAGS)

# Rule for generating dependency files (e.g. which headers were included)
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	mkdir -p $(DEPDIR)
	$(CC) -M $< $(CPPFLAGS) > $@.temp
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.o $@ : ,g' < $@.temp >  $@
	rm -rf $@.temp

clean:
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
	rm -rf $(TARGET)

ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS)
endif
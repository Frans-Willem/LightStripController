CC=g++
CPPFLAGS=-I./include
DEPS=
OBJDIR=obj
SRCDIR=src
DEPDIR=depends
SRCS=loader.cpp
OBJS=$(patsubst %, $(OBJDIR)/%,$(SRCS:.cpp=.o))
DEPS=$(patsubst %, $(DEPDIR)/%,$(SRCS:.cpp=.d))
TARGETPATH=../bin
TARGET=$(TARGETPATH)/loader

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(TARGETPATH)
	$(CC) -o $@ $^ -ldl

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CPPFLAGS)

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

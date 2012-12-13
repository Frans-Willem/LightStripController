CC=g++
CPPFLAGS=-I./include -I../common/include -fPIC
DEPS=
OBJDIR=obj
SRCDIR=src
DEPDIR=depends
SRCS=main.cpp CEvent.cpp CSizedQueue.cpp
OBJS=$(patsubst %, $(OBJDIR)/%,$(SRCS:.cpp=.o))
DEPS=$(patsubst %, $(DEPDIR)/%,$(SRCS:.cpp=.d))
TARGETPATH=../bin
TARGET=$(TARGETPATH)/core.so

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(TARGETPATH)
	$(CC) -shared -o $@ $^ -lpthread

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

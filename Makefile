CXX=g++

CXXFLAGS=-g -Wall `pkg-config --cflags libconfig++`
LNKFLAGS=`pkg-config --libs libconfig`

PRG_PREFIX=/usr/local/gccring
CFG_PREFIX=$(PRG_PREFIX)/cfg
CMD_PREFIX=$(PRG_PREFIX)/bin
BIN_PREFIX=/bin

OBJS=	\
	main.o	\
	env.o	\
	args.o	\
	collect.o	\
	utils.o	\
	cfg.o	\
	refine.o	\
	trace.o	\

TARGET=gccring

all:$(TARGET)


$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) $(LNKFLAGS) -o $@ $(OBJS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install:$(TARGET)
	install $(TARGET) $(BIN_PREFIX)/$(TARGET)
	install -d $(CFG_PREFIX)
	install gccring.cfg $(CFG_PREFIX)/gccring.cfg
	install -d $(CMD_PREFIX)

uninstall:
	@rm $(BIN_PREFIX)/$(TARGET)
	@rm -fr $(PRG_PREFIX)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJS)



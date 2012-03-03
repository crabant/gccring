CXX=g++

CXXFLAGS=-g -Wall
LNKFLAGS=

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

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJS)



CXX = mpiCC

SRCS    := $(wildcard *.cpp *.c src/*.cpp src/*c)
OBJS     = $(addprefix bin/obj/,$(notdir $(SRCS:.cpp=.o)))
OBJS     = $(addprefix bin/obj/,$(notdir $(SRCS:.c=.o)))

CXXFLAGS = -Wall -pedantic -O2 -fopenmp 
INCLUDES = -I/usr/include/

LDFLAGS  = #
LDLIBS   = -lm#-l/usr/lib/openmpi/ #-lpthread

OBJDIR   = bin/obj

TARGET   = run
CUR_DIR  = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
DESTDIR  = $(CUR_DIR)/bin/

DEPFILE  = Make.depend
DEPDIR   = bin/dep

.PHONY: clean distclean

#all: depend $(TARGET)
all: $(TARGET)
	@echo Installing $(TARGET) into $(DESTDIR)
	@mkdir -p $(DESTDIR)
#	@install -m 0755 $(TARGET) $(DESTDIR)

$(TARGET) : $(OBJS)
	@echo Linking $(TARGET)...
#	@$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

bin/obj/%.o: src/%.cpp
	@echo Compiling $<...
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(includes) -c -o $@ $<

bin/obj/%.o: src/%.c
	@echo Compiling $<...
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(includes) -o $(TARGET)  $<
	@echo "$(CXX) $(CXXFLAGS) $(includes) -o $(TARGET)  $<"
#	@echo "$(CXX) $(CXXFLAGS) $(includes) -c -o $@ $<"
#	@$(CXX) $(CXXFLAGS) $(includes) -c -o $@ $<

clean :
	$(RM) -r $(OBJDIR)
	$(RM) -r $(DEPDIR)
	$(RM) $(TARGET)

distclean: clean
	$(RM) $(DESTDIR)/$(TARGET)


#dependences
depend: $(DEPDIR)/$(DEPFILE)
	@echo Depend
	@touch $(DEPDIR)/$(DEPFILE)

$(DEPDIR)/$(DEPFILE):
	@echo Makefile - building dependencies in: $@
	@mkdir -p $(DEPDIR)
	@$(CXX) -E -MM $(CXXFLAGS) $(INCLUDES) $(SRCS) >> $@

# This make file will compile all .cpp and .c files in the directory. Be aware, the C++ compiler is used for both C and C++ files. Resulting objects are then linked into a single executable.

# make settings
NEWLINE=1 #newline after each object
ifdef NEWLINE
	NEWLINE = @echo ""
endif

# compile settings
CXX=g++
LINK=g++
PROGRAM=a.out
DEL=rm -rf
LIBS=-lm -fopenmp
WARN=-w
OP=-Ofast
OP_DEBUG=-O0
FLAGS= $(WARN) $(OP) -g

# setup object list
LOCAL_CPP_OBJECTS= $(patsubst %.cpp, %.o, $(wildcard ./*.cpp))
LOCAL_C_OBJECTS= $(patsubst %.c, %.o, $(wildcard ./*.c))
OBJ_LOADER_OBJECTS= $(patsubst %.cpp, %.o, $(wildcard ./obj/*.cpp))
PPM_OBJECTS= $(patsubst %.cpp, %.o, $(wildcard ./simplePPM/*.cpp))
RAY_OBJECTS= $(patsubst %.cpp, %.o, $(wildcard ./Rays/*.cpp))
LOADER_OBJECTS= $(patsubst %.c, %.o, $(wildcard ./obj_loader/*.cpp)) # the OBJ loader sub dir
OBJECTS=$(LOCAL_C_OBJECTS) $(LOCAL_CPP_OBJECTS) $(LOADER_OBJECTS) $(OBJ_LOADER_OBJECTS) $(PPM_OBJECTS) $(RAY_OBJECTS)

# make targets
makeall:
	make $(PROGRAM)
	$(NEWLINE)
all:
	make $(PROGRAM)
	$(NEWLINE)

$(PROGRAM): $(OBJECTS) 
	@echo "--$(PROGRAM)--"
	$(LINK) $(FLAGS) $(OBJECTS) $(LIBS) -o $(PROGRAM)

%.o: %.cpp
	@echo "--$@--"
	$(CXX) -c $(FLAGS) $< $(LIBS) -o $@
	$(NEWLINE)

%.o: %.c
	@echo "--$@--"
	$(CXX) -c $(FLAGS) $< $(LIBS) -o $@
	$(NEWLINE)

debug: %.cpp
	@echo "--Debug Build of $(PROGRAM) --"
	$(CXX) -c $(WARN) $(OP_DEBUG) -g $< $(LIBS) -o $@
	$(LINK) $(WARN) $(OP_DEBUG) -g $(OBJECTS) $(LIBS) -o $(PROGRAM)

# clean
.PHONEY: clean
clean:
	$(DEL) $(OBJECTS)

.PHONEY: cleanall
cleanall:
	$(DEL) $(OBJECTS)
	$(DEL) $(PROGRAM)

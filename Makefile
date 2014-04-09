CPP=		c++
FLAGS=		-std=c++11 -O3 -pthread -Wall -pedantic
EXEC=		hercule-server
BIN_DIR=	bin
WORK_DIR=	work
SRC_LIST=	$(wildcard *.cc)
OBJ_LIST=	$(SRC_LIST:.cc=.o)
OBJ=		$(OBJ_LIST:%=$(WORK_DIR)/%)

ifeq ($(DEBUG), yes)
	CPPFLAGS=$(FLAGS) -g
else
	CPPFLAGS=$(FLAGS)
endif

all: $(EXEC)

$(EXEC): $(OBJ_LIST)
	mkdir -p bin
	$(CPP) $(CPPFLAGS) -o $(BIN_DIR)/$@ $(OBJ)

%.o: %.cc
	mkdir -p work
	$(CPP) $(CPPFLAGS) -o $(WORK_DIR)/$@ -c $<

clean:
	rm -Rf $(WORK_DIR)
	rm -Rf $(BIN_DIR)


CC = gcc -c
LD = gcc -o
YACC = bison -y
RM = rm -f

APP = sq_checker
TEST_APP = sq_checker_test

#COVERAGE=1
# adjust executables names for Cygwin
ifdef ($(WINDIR))
APP = sq_checker.exe
TEST_APP =sq_checker_test.exe
endif

LIBS = ../lib
TEST_LIBS = libembUnit.a libtextui.a

SRCD  = ../src
SRCMD = ../src/main # main function source dir
SRCTD = ../tests    # unit tests source dir(s)
INCD  = ../include . ../lib

GRAM  = $(wildcard $(addsuffix /*.y, $(SRCD)))
SRC   = $(wildcard $(addsuffix /*.c, $(SRCD)))

SRCM  = $(wildcard $(addsuffix /*.c, $(SRCMD)))
SRCT  = $(wildcard $(addsuffix /*.c, $(SRCTD)))
INCLUDES = $(addprefix -I, $(INCD))

YFLAGS = -v -d --no-lines
CFLAGS = -g $(INCLUDES)
LDFLAGS = -g $(addprefix -L, $(LIBS))

ifeq ($(COVERAGE),1)
CFLAGS +=  -fprofile-arcs -ftest-coverage
endif 


# grammar must be built first, since we need y.tab.h for other sources
OBJ  = $(patsubst %.y,%.o,$(notdir $(GRAM)))
OBJ += $(patsubst %.c,%.o,$(notdir $(SRC)))
OBJM = $(patsubst %.c,%.o,$(notdir $(SRCM)))
OBJT = $(patsubst %.c,%.o,$(notdir $(SRCT)))
COV  = $(patsubst %.c,%.gcov,$(notdir $(SRC)))
COV  += $(patsubst %.y,%.gcov,$(notdir $(GRAM)))
COV  += $(patsubst %.y,%.gcov,$(notdir $(SRCT)))

VPATH = $(SRCD) $(SRCMD) $(SRCTD) $(LIBS)

.PHONY: all

build: clean check all

all: $(APP)

coverage: 
	make $(COV)

test:
	@echo $(APP)
	@echo $(OBJ) $(OBJT)

libembUnit.a:
	@cd $(LIBS)/embUnit; make clean all

libtextui.a:
	@cd $(LIBS)/textui; make clean all

check: $(TEST_LIBS) $(TEST_APP) 
	@./$(TEST_APP) | tee test.log | grep construct_path
	@#bash -c "if [ ! \"`grep '^OK (' test.log`\" ]; then echo ---TESTS FAILED---; else echo +++TESTS PASSED+++; fi"

%.gcov: %.o
	gcov -b -f $<
     
%.o: %.c
	$(CC) $(CFLAGS) $<

$(TEST_APP): $(OBJ) $(OBJT)
	@$(LD) $@ $+ $(LDFLAGS) -lembUnit -ltextui -lgcov
	@$(RM) ../msvc/parser/*.*

$(APP): $(OBJ) $(OBJM)
	@$(LD) $@ $(LDFLAGS) $+ -lgcov
	@$(RM) ../msvc/parser/*.*

wc:
	wc -l $(SRC) $(SRCT) $(SRCM)

clean:
	$(RM) $(TEST_APP) $(OBJ) $(OBJT) $(OBJM) ../msvc/parser/* *.gcov *.gcda *.gcno test.log *.output y.tab.h $(patsubst %.y,%.c,$(notdir $(GRAM)))

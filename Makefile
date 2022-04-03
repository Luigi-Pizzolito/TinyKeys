PNAME=TinyKeys
SRCD=src/*.cpp
ODIR=bin

C =gcc
CC=g++
IDIR=dep/include
LDIR=dep/lib
LIBS=-l sqlite3
CFLAGS=-std=c++17 -I $(IDIR) -I src/


$(ODIR)/$(PNAME): $(SRCD)
	@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	@echo "\033[0m\033[0;32mBuilding Project..........................\033[0m\033[2m"
	$(CC) $^ -o $(ODIR)/$(PNAME) $(CFLAGS) $(LIBS)
	@echo "\033[0m\033[1;32mProject Compiled Successfully.\033[0m"

run: $(ODIR)/$(PNAME)
	@echo "\033[0;34mRunning Project..........................\033[0m\033[2m\033[39;2m"
	./$(ODIR)/$(PNAME)
	@echo "\033[0m\033[1;34mFinished.\033[0m"


lib:
	@echo "\033[1;33mBuilding Libraries..........................\033[0m\033[2m\033[39;2m"
	cd dep/src/sqlite; $(C) -DSQLITE_OMIT_LOAD_EXTENSION -dynamiclib sqlite3.c -install_name sqlite3 -I . -lpthread -lm -o ../../lib/sqlite3.dylib; cp sqlite3.h ../../include/;


.PHONY: clean

clean:
	@echo "\033[0;33mCleaning Project..........................\033[0m\033[2m"
	rm -vf $(ODIR)/*
	@echo "\033[0m\033[0;33mCleaned Project.\033[0m\033[2m"

clean-all:
	@echo "\033[0;33mCleaning ALL Project..........................\033[0m\033[2m"
	rm -vf $(ODIR)/*
	rm -vf $(IDIR)/*
	rm -vf $(LDIR)/*
	@echo "\033[0m\033[0;33mCleaned ALL Project.\033[0m\033[2m"
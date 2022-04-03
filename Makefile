PNAME=TinyKeys
SRCD=src/*.cpp src/**/*.cpp
ODIR=bin

C =gcc
CC=g++
IDIR=dep/include
LDIR=dep/lib
LIBS=-lsqlite3 -lfltk -lXext -lX11 -lm -lpthread -framework Cocoa -framework ApplicationServices -framework Carbon
CFLAGS=-I $(IDIR) -I src/ -g


$(ODIR)/$(PNAME): $(SRCD)
	@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	@echo "\033[0m\033[0;32mBuilding Project..........................\033[0m\033[2m"
	$(CC) $^ -o $(ODIR)/$(PNAME) $(CFLAGS) -L$(LDIR) $(LIBS)
	cp src/empty.db bin/keys.db;
	@echo "\033[0m\033[1;32mProject Compiled Successfully.\033[0m"

run: $(ODIR)/$(PNAME)
	@echo "\033[0;34mRunning Project..........................\033[0m\033[2m\033[39;2m"
	cd ./$(ODIR); ./$(PNAME) 2> /dev/null
	@echo "\033[0m\033[1;34mFinished.\033[0m"


lib:
	@echo "\033[1;33mBuilding Libraries..........................\033[0m\033[2m\033[39;2m"
	@echo "\033[1;33mBuilding SQLite3............................\033[0m\033[2m\033[39;2m"
	# cd dep/src/sqlite; $(C) -DSQLITE_OMIT_LOAD_EXTENSION -dynamiclib sqlite3.c -install_name sqlite3 -I . -lpthread -lm -o ../../lib/sqlite3.dylib; cp sqlite3.h ../../include/;
	cd dep/src/sqlite; $(C) -c -DSQLITE_OMIT_LOAD_EXTENSION -o sqlite3.o sqlite3.c -I . ; ar rcs ../../lib/sqlite3.a sqlite3.o; rm sqlite3.o; cp sqlite3.h ../../include/;
	@echo "\033[1;33mBuilding FLTK...............................\033[0m\033[2m\033[39;2m"
	sleep 5
	cd dep/src/; tar -xf fltk-1.3.8-source.tar.gz; mkdir fltk; PDIR="$(pwd)/fltk"; cd fltk-1.3.8; ./configure --prefix=$PDIR; make; make install; make clean; cd ..; rm -r fltk-1.3.8; cd fltk; cp -r include/* ../../include/; cp lib/* ../../lib/; cd ../; rm -r fltk;


.PHONY: clean

clean:
	@echo "\033[0;33mCleaning Project..........................\033[0m\033[2m"
	rm -vrf $(ODIR)/*
	@echo "\033[0m\033[0;33mCleaned Project.\033[0m\033[2m"

clean-all:
	@echo "\033[0;33mCleaning ALL Project..........................\033[0m\033[2m"
	rm -vf $(ODIR)/*
	rm -vf $(IDIR)/*
	rm -vf $(LDIR)/*
	@echo "\033[0m\033[0;33mCleaned ALL Project.\033[0m\033[2m"
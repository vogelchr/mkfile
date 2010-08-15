CPPFLAGS=-D_FILE_OFFSET_BITS=64
CFLAGS=-Wall -g
LDFLAGS=-g

PREFIX=/usr/local
MANDIR=$(PREFIX)/share/man/man1
BINDIR=$(PREFIX)/bin

all : mkfile

mkfile : mkfile.o

.PHONY : clean install
clean :
	rm -f *~ *.o mkfile

install : mkfile
	install -m755 mkfile $(BINDIR)/mkfile
	install -d -m755 $(MANDIR)
	install -m644 mkfile.1 $(MANDIR)/mkfile.1

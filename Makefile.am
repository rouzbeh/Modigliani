AUTOMAKE_OPTIONS = foreign

SUBDIRS = src test

all:
	make -C ./src
	make -C ./test
install:
	make -C ./src install
uninstall:
	make -C ./src uninstall
clean:
	make -C ./src clean
test:
	make -C ./test test

EXTRA_DIST = AUTHORS LICENCE README.md modigliani.lsm Modigliani.Doxyfile

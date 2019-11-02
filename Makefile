all:    lib

src/libs.a:
	make -C src

lib:    src/libs.a

clean:
	cd samples;find . -name "*.o" -exec rm -rf {} \;
	cd src; make clean

sample:
	make -C samples

doc:
	cd manual; doxygen Doxyfile



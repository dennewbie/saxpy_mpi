all: clean saxpy
	
saxpy: saxpy.o UsageUtility.o SaxpyLibrary.o
	mpicc saxpy.o UsageUtility.o SaxpyLibrary.o -o saxpy
saxpy.o: saxpy.c UsageUtility.h SaxpyLibrary.h
	mpicc -c saxpy.c
	


UsageUtility.o: UsageUtility.c UsageUtility.h
	mpicc -c UsageUtility.c

SaxpyLibrary.o: UsageUtility.c UsageUtility.h SaxpyLibrary.c SaxpyLibrary.h
	mpicc -c UsageUtility.c SaxpyLibrary.c



clean:
	rm -f *.o all
	rm -f saxpy

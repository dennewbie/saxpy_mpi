all: clean saxpy
	
saxpy: saxpy.o UsageUtility.o SaxpyLibrary.o
	gcc saxpy.o UsageUtility.o SaxpyLibrary.o -o saxpy
	
saxpy.o: saxpy.c UsageUtility.h SaxpyLibrary.h
	gcc -c saxpy.c
	


UsageUtility.o: UsageUtility.c UsageUtility.h
	gcc -c UsageUtility.c

SaxpyLibrary.o: UsageUtility.c UsageUtility.h SaxpyLibrary.c SaxpyLibrary.h
	gcc -c UsageUtility.c SaxpyLibrary.c



clean:
	rm -f *.o all
	rm -f saxpy

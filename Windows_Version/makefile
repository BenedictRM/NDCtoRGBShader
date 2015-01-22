# CSCI 5239 NDC to RGB
EXE=HW1

# Main target
all: $(EXE)

#  MinGW, path defined in CFLG is the path to ODE header files on your system
#  The ode_singled.dll MUST be in the same directory as your executable!!!! Copy and paste it in there
ifeq "$(OS)" "Windows_NT"
INCL=-I"C:\Users\Russ Mehring\adt-bundle-windows-x86_64-20130522\ode-0.13\include"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lglew32 -lglut32cu -lglu32 -lopengl32 -lode_singled 
LINKFLAGS=-L"C:\Users\Russ Mehring\adt-bundle-windows-x86_64-20130522\ode-0.13\lib\DebugSingleDLL" 
CLEAN=del *.exe *.o *.a
endif

# Dependencies
NDCtoRGB2.o: NDCtoRGB2.c CSCIx239.h
fatal.o: fatal.c CSCIx239.h
loadtexbmp.o: loadtexbmp.c CSCIx239.h
print.o: print.c CSCIx239.h
project.o: project.c CSCIx239.h
errcheck.o: errcheck.c CSCIx239.h
ode.o: ode.c CSCIx239.h
object.o: object.c CSCIx239.h
elapsed.o: elapsed.c CSCIx239.h
fps.p: fps.c CSCIx239.h
shader.o: shader.c CSCIx239.h

#  Create archive
HW1.a:fatal.o loadtexbmp.o print.o project.o errcheck.o ode.o object.o elapsed.o fps.o shader.o
	ar -rcs $@ $^
	
# Compile rules
.c.o:
	gcc -c $(CFLG) $(INCL) $<
.cpp.o:
	g++ -c $(CFLG) $(INCL) $<

#  Compile and link
HW1:NDCtoRGB2.c HW1.a
	gcc -O3 -o $@ $^  $(CFLG) $(LIBS) $(LINKFLAGS) $(INCL) $(LDFLAGS) 

#  Clean
clean:
	$(CLEAN)

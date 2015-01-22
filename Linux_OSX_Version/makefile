# CSCI 5239 NDC to RGB
EXE=HW1

# Main target and ode target
all: ode $(EXE)

#  OSX, auto-loads ODE
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations -I/tmp/ode/bin/include
LIBS=-framework GLUT -framework OpenGL -lode
LDFLAGS= -L/tmp/ode/bin/lib
CLEAN=rm -f $(EXE); rm -rf ode /tmp/ode/ *.o *.a 
else
#  Linux/Unix/Solaris, auto loads ODE
CFLG=-O3 -Wall -funroll-loops -fomit-frame-pointer -I/tmp/ode/bin/include
LIBS=-lglut -lGLU -lGL -lm -lode
LDFLAGS= -L/tmp/ode/bin/lib
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE); rm -rf ode /tmp/ode/ *.o *.a 
endif

#To run, type 'make run' (On OSX or Linux command line)
run: HW1
	LD_LIBRARY_PATH=/tmp/ode/bin/lib ./HW1

#Download ode from internet into its own folder
ode:
	@echo Pulling tar file
	@wget https://bitbucket.org/odedevs/ode/downloads/ode-0.13.1.tar.gz
	@tar xzf ode-0.13.1.tar.gz
	@rm ode-0.13.1.tar.gz
	@mv ode-0.13.1/ /tmp/ode
	cd /tmp/ode/; ./configure --prefix=/tmp/ode/bin/ --enable-double-precision --enable-shared; make; make install;
	touch ode

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

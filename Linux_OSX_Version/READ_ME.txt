Author: Russell Mehring
Date: 01/21/2015
CSCI 5239

To Compile: type 'make'

To Run: Linux/OSX: Type 'make run'
        Windows: Type 'HW1'

To Clean: Type 'make clean'


*******************************************************
                         ODE
(This should also install automatically in the directory for Linux/OSX users)
*******************************************************
TO INSTALL OPEN DYNAMICS ENGINE (ODE) ON LINUX/UBUNTU permanently:
  1) Ideally you should only have to go here and call the needed sudo apt-get's for all 4 'libode' commands at the top page: https://launchpad.net/ubuntu/+source/ode
  2) Worst case you will need to go to: http://ode-wiki.org/wiki/index.php?title=Manual:_Install_and_Use, this will give you directions in case mine are inadequate
     
     1. From here dl the latest tar file from http://sourceforge.net/projects/opende/
     2. Extract this tar file
     3. cd ode-0.13 (The current version as of my install)
     4. ./configure --enable-double-precision --with-trimesh=opcode --enable-new-trimesh --enable-shared 
     5. make
     6. make install
     7. *****ODE should now be installed on your system, but if you decide you like it AND want python bindings you can also call:
     8. cd bindings/python/ 
     9. python setup.py install 
     10. ***BE SURE TO DEFINE YOUR VERSION AS SINGLE PRECISION, otherwise it will throw an error message with this code***

TO INSTALL OPEN DYNAMICS ENGINE (ODE) ON WINDOWS:

   1) Please read my blog post at: https://www.tumblr.com/blog/russellmehring
*******************************************************

CONTROLS:

(1) Zooming (If NOT in first person view)
    
    F1: Zoom-In
    F2: Zoom-Out

(2) Rotations/views
   
    Up Arrow: y-axis up/ look up
    Down Arrow: y-axis down/ look down
    Left Arrow: Turn x-axis left/ look left
    Right Arrow: Turn x-axis right/ look right

(3) Perspectives
    m/M to change from perspective to orthogonal view
		
(5) Additional keys
    
    Space: to drop skull on objects to show off physics  
    Esc: To close the program


Of Interest: Has physics! 

5) Thanks to Zach Anders for helping me get my makefile working with ode (BIG thanks!)

6) OBJ file 'mammoth' from http://tf3dm.com/3d-model/mammoth-90284.html

7) OBJ file 'Skull' from http://tf3dm.com/3d-model/skull-76849.html

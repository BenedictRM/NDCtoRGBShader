//The value of this vector should be coming in from vertex shader
//A fragment shader processes…fragments. A fragment is basically 
//a position on the window (X,Y), a depth value (Z), plus all the interpolated data from previous stages.

varying vec4 foo;

void main()
{
   //colors indiv. fragments
   gl_FragColor = foo;//Set this value with converted NDC values [0 1]
}

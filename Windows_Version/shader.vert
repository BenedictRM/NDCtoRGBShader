//  NDC to RGB vertex shader
//  A vertex shader operates on individual vertices, one vertex at a time

varying vec4 foo;//This is output that is set here, then fed into the shader.frag shader (output is READ ONLy)

void main()
{   
   //Multiply the position against the modelview matrix to place the vertex 
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   //Store gl_Position so that it does not change 
   foo = gl_Position;
   //Now do the perspective divide to get NDC coordinates (-1 to 1), ensure positive values by adding 1 and dividing by 2
   foo.xyz = (((foo.xyz / foo.w) + 1.0) / 2.0); 
}

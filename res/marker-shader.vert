uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec4 instanceColor;
uniform sampleBuffer matricesTex;
out vec4 color;

void main() {
  if (instanceColor.a <= 0.0) {
    color = vec4(0.0);
    gl_Position = vec4(0.0);

  } else {

    color = instanceColor;

    int matI = gl_InstanceID * 4;
    mat4 transformMatrix =
    mat4(texelFetch(matricesTex, matI+0),
         texelFetch(matricesTex, matI+1),
         texelFetch(matricesTex, matI+2),
         texelFetch(matricesTex, matI+3));

    gl_Position = modelViewProjectionMatrix * transformMatrix * position;
  }
}
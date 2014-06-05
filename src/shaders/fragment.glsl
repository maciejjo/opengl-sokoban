#version 120

//uniform vec3 triangleColor;
varying vec3 Color;
varying vec2 Texcoord;
varying vec3 Normal;
varying float lightIntensity;

uniform sampler2D tex_crate;
uniform sampler2D tex_shit;

varying vec4 l;
varying vec4 n;
varying vec4 v;

void main()
{

  vec4 n_l = normalize(l);
  vec4 n_n = normalize(n);
  vec4 n_v = normalize(v);
  vec4 r = reflect(-n_l, n_n);
  float nl = max(dot(n_n, n_l),0);
  float rv = pow(max(dot(r, n_v), 0), 3);

  gl_FragColor = vec4(Color, 1) * vec4(nl,nl,nl,1)+vec4(rv,rv,rv,1);

   //texture2D(tex_crate, Texcoord);
//  gl_FragColor = Color;
 // gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}

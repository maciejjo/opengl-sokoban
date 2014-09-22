#version 120

//uniform vec3 triangleColor;

varying vec3 Color;
varying vec2 Texcoord;
varying vec3 Normal;
varying float lightIntensity;

varying vec4 l;
varying vec4 n;
varying vec4 v;

uniform sampler2D tex;

void main()
{

  vec4 n_l = normalize(l);
  vec4 n_n = normalize(n);
  vec4 n_v = normalize(v);
  vec4 r = reflect(-n_l, n_n);
  float nl = max(dot(n_n, n_l),0);
  float rv = pow(max(dot(r, n_v), 0), 3);

  gl_FragColor = texture2D(tex, Texcoord) * vec4(nl,nl,nl,1) +vec4(rv,rv,rv,1);

   //texture2D(tex_crate, Texcoord);
//  gl_FragColor = Color;
 // gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}

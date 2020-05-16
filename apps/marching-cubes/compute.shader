#version 430

layout(local_size_x = 1024) in;

layout(std430, binding=1) buffer trs
{
    int triTable[256][16];
};

layout(std430, binding=2) buffer pos
{
    vec3 vertices[ ];
};


uniform vec3  chunk_location;
uniform int   chunk_size;
uniform float block_size;
uniform float threshold;

uint gid = gl_GlobalInvocationID.x;

//	Simplex 3D Noise 
//	by Ian McEwan, Ashima Arts
//
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}

float snoise(vec3 v){ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //  x0 = x0 - 0. + 0.0 * C 
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

// Permutations
  i = mod(i, 289.0 ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients
// ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

float getValue(vec3 position)
{
    return (snoise(position) + 1.0) / 2.0;
}

void main()
{
    const vec3 start_point = chunk_location * (float(chunk_size) * block_size);

    vec3 directions[] = {
        vec3( 1.f, -1.f,  1.f) * (block_size / 2.f),
        vec3( 1.f, -1.f, -1.f) * (block_size / 2.f),
        vec3(-1.f, -1.f, -1.f) * (block_size / 2.f),
        vec3(-1.f, -1.f,  1.f) * (block_size / 2.f),
        vec3( 1.f,  1.f,  1.f) * (block_size / 2.f),
        vec3( 1.f,  1.f, -1.f) * (block_size / 2.f),
        vec3(-1.f,  1.f, -1.f) * (block_size / 2.f),
        vec3(-1.f,  1.f,  1.f) * (block_size / 2.f)
    };

    uint vert_index = 0;
    for (int x = 0; x < chunk_size; x++)
        for (int y = 0; y < chunk_size; y++)
            for (int z = 0; z < chunk_size; z++)
            {
                const vec3 position = vec3(x, y, z) * block_size + start_point;

                uint index = 0;
                for (int i = 0; i < 8; i++)
                {
                    vec3  point_pos = position + directions[i];
                    float value = getValue(point_pos);
                    if (value < threshold) continue;

                    index |= 1 << i;
                }
                
                for (int i = 0; triTable[index][i] != -1; i++)
                {
                    int edgeIndex = triTable[index][i];
                    int points[2] = { -1, -1 };
                    /**/ if (edgeIndex < 4)
                    {
                        points[0] = edgeIndex;
                        points[1] = (edgeIndex + 1) % 4;
                    }
                    else if (edgeIndex >= 3 && edgeIndex < 8)
                    {
                        points[0] = edgeIndex;
                        points[1] = ((edgeIndex - 3) % 4) + 4;
                    }
                    else
                    {
                        points[0] = edgeIndex - 8;
                        points[1] = edgeIndex - 4;
                    }

                    float values[2] = {
                        getValue(directions[points[0]] + position),
                        getValue(directions[points[1]] + position)
                    };

                    float t = (0.5f - values[0]) / (values[1] - values[0]);
                    vec3 pos = directions[points[0]] + ((directions[points[0]] - directions[points[1]]) * t);


                    vertices[vert_index++] = pos * 2.0 + position;
                    vertices[gl_GlobalInvocationID.x] = vec3(1, 2, 3);
                    //Vertex vert;
                    //vert.position = pos * 2.f + position;
                    //vert.color = color;
                    //va.push(vert);
                }
            }
}

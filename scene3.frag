#version 120

// Ray marching modeling parameters 
#define MIN_DISTANCE 0.01
#define MAX_ITERATION 100
#define MAX_DISTANCE 50

// Ray marching modules
#define SHADOWS 0
#define GLOW 1
#define LIGHT_POS 0

// Colors
#define FAR_COLOR vec3(0.5, 0.78, 1) // 127 199 255
#define OBJECT_COLOR vec3(1, 0.81, 0.28) // 255 207 72
#define GLOW_COLOR vec3(1, 1, 1) // 255 255 255

// Shaders uniform
uniform vec2 screenSize;
uniform float psiCamera;
uniform float phiCamera;
uniform vec3 pCamera;
uniform vec3 light;
uniform float time;

// Plane sdf
float planeSdf(vec3 pos)
{
    return pos.z;
}

// Sphere sdf
float sphereSdf(vec3 pos, float rad)
{
    return length(pos) - rad;
}

// Box sdf
float boxSdf(vec3 pos, vec3 par)
{
    vec3 q = abs(pos) - par;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float torusSdf( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

// Signed distance function
float sdf(vec3 pos)
{
    float d = 1e10;

    // Sphere
    {
        vec3 p = mod(pos, 2) - vec3(1, 1, 1);
        d = min(d, sphereSdf(p, 0.5));
    }

    return d;
}

// Get normal from sdf
vec3 getNormal(vec3 p)
{
    vec2 dn = vec2(0.01, 0.0);
    float d = sdf(p);
    vec3 n = d - vec3(
        sdf(p-dn.xyy),
        sdf(p-dn.yxy),
        sdf(p-dn.yyx)
    );

    return normalize(n);
}

// A function that realise ray marching algorithm
vec2 rayMarch(vec3 _pCamera, vec3 _n)
{
    float t = 0;
    vec3 col = vec3(0, 0, 0);
    int i;
    for (i = 0; i < MAX_ITERATION; i++)
    {
        vec3 p = _pCamera + _n*t;  // Position
        float dt = sdf(p);  // Distance

        if (dt < MIN_DISTANCE)
            return vec2(t, i);

        if (t > MAX_DISTANCE)
            return vec2(MAX_DISTANCE, i);

        t += dt;
    }

    return vec2(t, i);
}

// A function that return light of object surface
float getLight(vec3 pos, vec3 lig)
{
    float ligInt = 5;
    vec3 n = getNormal(pos);

#if LIGHT_POS
    vec3 nL = normalize(lig-pos);
    float dL = length(lig-pos);
    float rat = (ligInt-dL)/ligInt;
    float dif = clamp(dot(n, lig)*rat, 0, 1);
#else
    vec3 nL = lig;
    float dL = MAX_DISTANCE;
    float dif = clamp(dot(n, lig), 0, 1);
#endif

#if SHADOWS
    vec2 res = rayMarch(pos + n*0.1, nL);
    float dis = res.x;
    if (dis < dL)
        dif *= 0.1;
#endif

    return dif;
}

// A function that converts from one color to another by parametr from { bottom to top }
vec3 colorConv(vec3 fColor, vec3 sColor, float bot, float top, float par)
{ 
    vec3 rColor; // Result color
    rColor = (sColor - fColor)*(par - top)/(top - bot) + sColor;
    return rColor;
}

// A function that creates glow effect
vec3 glow(vec3 color, float itr)
{
#if GLOW
    float bot = 0.0;
    float top = 1;
    float f = clamp(itr/MAX_ITERATION,bot,top);
    return colorConv(color, GLOW_COLOR, bot, top, f);
#else
    return color;
#endif
}

// A function that color far distance
vec3 far(vec3 color, float dis)
{
    float f = dis/MAX_DISTANCE;
    return colorConv(color, FAR_COLOR, 0, 1, f);
}

// Main function of shader
void main()
{
    // Taking position vector of screen pixel
    vec2 ss = screenSize;
    vec2 uv = vec2(gl_FragCoord.x/ss.x, gl_FragCoord.y/ss.y);
    uv = 2*uv-vec2(1.0, 1.0);
    uv.y = uv.y * ss.y/ss.x;

    // Initializaton of uniform parameters
    float psiC = psiCamera;
    float phiC = phiCamera;
    vec3 pC = pCamera;
    vec3 nL = light;
    float t = time;

    // Camera movement
    vec3 nC = normalize(vec3(1,-uv.x,uv.y));
    vec3 nCn = nC;
    nC.x = cos(psiC)*cos(phiC)*nCn.x - sin(psiC)*nCn.y - cos(psiC)*sin(phiC)*nCn.z;
    nC.y = sin(psiC)*cos(phiC)*nCn.x + cos(psiC)*nCn.y - sin(psiC)*sin(phiC)*nCn.z;
    nC.z = sin(phiC)*nCn.x + cos(phiC)*nCn.z;

    // Computing ray marching
    vec2 res = rayMarch(pC, nC);
    float dis = res.x;
    float itr = res.y;
    vec3 ps = pC + nC*dis;
    float dif = getLight(ps, nL);

    // Setting color
    vec3 color = far(glow(OBJECT_COLOR*dif, itr), dis);
    gl_FragColor = vec4(color, 1.0);
}
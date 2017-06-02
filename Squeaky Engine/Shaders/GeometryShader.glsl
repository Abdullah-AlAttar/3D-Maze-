#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4f;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // First vertex normal
    GenerateLine(1); // Second vertex normal
    GenerateLine(2); // Third vertex normal
}  
حمد زكريا رفاعى عبد العال   2014170027   1
عبد الله شعبان السيد طه   2014170604  11
عبد الله عبد القادر رشدى محمد   2014170278  11
عبد الله محمد فخرى عبد الرحيم العطار  2014170551  11
عبدالله محمود عبدالله عبدربه   2014170279  11
cfa_reg!=-1 not met
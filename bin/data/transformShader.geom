#version 150

layout(points) in;
layout(points, max_vertices = 1) out;

in vec3[] vPosition;
out vec3 gPosition;

void main()
{
    gPosition = vPosition[0];
    EmitVertex();
    EndPrimitive();
}
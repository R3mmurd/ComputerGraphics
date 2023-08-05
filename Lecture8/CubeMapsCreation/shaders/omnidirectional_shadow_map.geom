#version 410

const int NUM_FACES = 6;
const int NUM_VERTICES_PER_TRIANGLE = 3;

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 light_matrices[NUM_FACES];

out vec4 fragment_position;

void main()
{
    for (int face = 0; face < NUM_FACES; ++face)
    {
        gl_Layer = face;

        for (int i = 0; i < NUM_VERTICES_PER_TRIANGLE; ++i)
        {
            fragment_position = gl_in[i].gl_Position;
            gl_Position = light_matrices[face] * fragment_position;
            EmitVertex();
        }

        EndPrimitive();
    }
}
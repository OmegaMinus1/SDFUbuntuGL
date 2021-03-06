#include "mesh.h"
#include <vector>

Mesh::Mesh(Vertex* vertices, unsigned int numberOfVertices)
{
    m_drawCount = numberOfVertices;

    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textCoords;

    positions.reserve(numberOfVertices);
    textCoords.reserve(numberOfVertices);

    for(size_t i =0; i< numberOfVertices; i++)
    {
        positions.push_back(*vertices[i].GetPos());
        textCoords.push_back(*vertices[i].GetTexCoord());
    }

    //Positions
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //TexCoords
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vertexArrayObject);

}

void Mesh::Draw()
{
    glBindVertexArray(m_vertexArrayObject);

    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

    glBindVertexArray(0);
}





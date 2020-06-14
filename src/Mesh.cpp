#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	:vertices(vertices), indices(indices), textures(textures)
{
	setup();
}



void Mesh::draw(Shader &shader) 
{
	bindTexture(shader);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);	// unbind VAO
	glActiveTexture(GL_TEXTURE0);	// set texture config back to default
}


void Mesh::setup() 
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	// VAO 
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	// vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	// vertex texture coordinate
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	// vertex tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(3);

	// vertex bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(VAO);
}


void Mesh::bindTexture(Shader& shader) 
{
	unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) 
	{
		glActiveTexture(GL_TEXTURE0 + i);	// active texture

		// name of texture: type + number   ex: texture_specular3
		std::string type = textures[i].type;
		std::string number;

		if (type == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (type == "texture_specular")
			number = std::to_string(specularNr++);
		else if (type == "texture_normal")
			number = std::to_string(normalNr++);
		else if (type == "texture_height")
			number = std::to_string(heightNr++);
		else
			std::cout << "Error, undecleared texture type, type name: " << type << std::endl;

		// pass texture unit number to sampler2D
		glUniform1i(shader.getUniformLocation((type + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}


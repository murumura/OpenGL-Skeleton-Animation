#include "mesh.h"

#include <iostream>
void Mesh::setupMesh()
{
	// create buffers/arrays
	vertex_array_ = Array::create();
	vertex_array_->bind();
	array_buffer_ = Buffer::create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices_);
	array_buffer_->bind();
	/** check if indices vector is given?! */
	if (!indices_.empty()) {
		element_buffer_ = Buffer::create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices_);
		element_buffer_->bind();
	}
	// set the vertex attribute pointers
	// ---------------------------------
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));

	// vertex normal coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void *)(offsetof(Vertex, bone_ids) + 0 * sizeof(int)));
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void *)(offsetof(Vertex, bone_ids) + 4 * sizeof(int)));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, bone_weights) + 0 * sizeof(float)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, bone_weights) + 4 * sizeof(float)));

	// unbind vertex attribute array
	vertex_array_->unbind();
	array_buffer_->unbind();
	element_buffer_->unbind();
}
void Mesh::Draw(const std::shared_ptr<Program> &shader_program)
{
	// active proper texture unit before binding texture
	glActiveTexture(GL_TEXTURE0);
	shader_program->set_uniform("uDiffuseTexture", 0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// draw mesh
	vertex_array_->bind();
	glDrawArrays(GL_TRIANGLES, 0, indices_.size());
	vertex_array_->unbind();
	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}
glm::mat4 Mesh::mat4_from_aimatrix4x4(aiMatrix4x4 matrix)
{
	glm::mat4 res;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) res[j][i] = matrix[i][j];
	return res;
}
#ifndef MESH_H
#define MESH_H
#include <memory>
#include <string>
#include <vector>

#include "array.h"
#include "buffer.h"
#include "err.h"
#include "program.h"
#include "shader.h"
#include "texture_manager.h"
#include "typedef.h"
class Mesh {
   public:
	Mesh()
	{
		vertices_.clear();
		indices_.clear();
	}
	~Mesh()
	{
		glDeleteTextures(1, &texture_id);
	}
	template <typename... Args>
	static std::shared_ptr<Mesh>
	create(Args... args)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->build(args...);
		return mesh;
	}

	// initializes all the buffer objects/arrays
	void setupMesh();
	// render the mesh
	void Draw(const std::shared_ptr<Program> &shader_program);

	template <typename... Vertices>
	void build(Vertices... vertices_args)
	{
		_build(vertices_args...);
	}
	template <typename... Vertices>
	void _build(std::vector<Vertex> &vertices, Vertices... vertices_args)
	{
		vertices_ = vertices;
		_build(vertices_args...);
	}
	template <typename... Vertices>
	void _build(std::vector<uint32_t> &indices, Vertices... vertices_args)
	{
		indices_ = indices;
		_build(vertices_args...);
	}

	template <typename... Vertices>
	void _build(const std::string &texture_path, Vertices... vertices_args)
	{
		texture_id = TextureManager::LoadTexture(texture_path);
		_build(vertices_args...);
	}
	void _build()
	{
		setupMesh();
	}
	glm::mat4 mat4_from_aimatrix4x4(aiMatrix4x4 matrix);

   protected:
	// mesh Data
	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	uint32_t texture_id;
	// buffer data
	std::shared_ptr<Array> vertex_array_;
	std::shared_ptr<Buffer> element_buffer_, array_buffer_;
};
#endif
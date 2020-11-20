#ifndef MODEL_H
#define MODEL_H
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "camera.h"
#include "err.h"
#include "mesh.h"
#include "program.h"
#include "shader.h"
#include "stbimg_wrapper.h"
#include "typedef.h"
#include "util.h"
namespace fs = std::filesystem;
using namespace Assimp;
class Model {
   public:
	// constructor, expects a filepath to a 3D model.
	Model(const std::string &file_path)
	{
		load_model(file_path);
	}
	~Model()
	{
		aiReleaseImport(scene);
	}
	template <typename... Args>
	static std::shared_ptr<Model>
	create(const std::string &file_path, Args... args)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>(file_path);
		model->build(args...);
		return model;
	}
	template <typename... Args>
	void build(Args... args)
	{
		_build(args...);
	}
	template <typename... Args>
	void _build(const std::string v_shader_path, const std::string f_shader_path, Args... args)
	{
		shader_program = Program::create(
		    Shader::create_from_file(GL_VERTEX_SHADER, v_shader_path),
		    Shader::create_from_file(GL_FRAGMENT_SHADER, f_shader_path));
		_build(args...);
	}
	void _build()
	{
	}
	void load_model(const std::string &model_file_path);
	void processNode(aiNode *node, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
	void processMesh(aiMesh *mesh, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
	void add_mesh(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, const std::string &texture_path);
	// draws the model, and thus all its meshes
	void Draw();
	void Draw(uint32_t animation_id,
	          const std::weak_ptr<Camera> &camera_ptr,
	          const float width,
	          const float height);
	void Draw(uint32_t animation_id,
	          const glm::mat4 &view_matrix,
	          const glm::mat4 &proj_matrix);

   protected:
	std::string directory_path;
	Assimp::Importer importer;
	const aiScene *scene;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::shared_ptr<Program> shader_program{nullptr};
	Bone_Namer bone_namer;
	std::vector<glm::mat4> bone_matrices, bone_offsets;
	std::map<std::pair<uint32_t, std::string>, uint32_t> animation_channel_map;

	void RecursivelyUpdateBoneMatrices(int animation_id, aiNode *node, glm::mat4 transform, double ticks);
	static glm::mat4 InterpolateTranslationMatrix(aiVectorKey *keys, uint32_t n, double ticks);
	static glm::mat4 InterpolateRotationMatrix(aiQuatKey *keys, uint32_t n, double ticks);
	static glm::mat4 InterpolateScalingMatrix(aiVectorKey *keys, uint32_t n, double ticks);
	glm::mat4 mat4_from_aimatrix4x4(aiMatrix4x4 matrix)
	{
		glm::mat4 res;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) res[j][i] = matrix[i][j];
		return res;
	}
};
#endif
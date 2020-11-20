#ifndef TYPE_DEF_H
#define TYPE_DEF_H
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <array>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "err.h"
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
constexpr int NUM_BONES_PER_VEREX = 10;
using Vec3_t = glm::vec3;
using Vec2_t = glm::vec2;
using Mat4_t = glm::mat4;
struct Vertex {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
	// normal
	glm::vec3 Normal;
	int bone_ids[NUM_BONES_PER_VEREX];
	float bone_weights[NUM_BONES_PER_VEREX];
	Vertex()
	{
		std::fill(bone_ids, bone_ids + NUM_BONES_PER_VEREX, 0);
		std::fill(bone_weights, bone_weights + NUM_BONES_PER_VEREX, 0);
	}
	void addBoneData(uint bone_id, float weight)
	{
		int i;
		for (i = 0; i < NUM_BONES_PER_VEREX; i++)
			if (bone_weights[i] == 0) break;
		if (i >= NUM_BONES_PER_VEREX) throw Error("MaxBoneExceededError");
		bone_weights[i] = weight;
		bone_ids[i] = bone_id;
	}
};
#endif
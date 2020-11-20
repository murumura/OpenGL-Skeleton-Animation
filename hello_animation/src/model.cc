#include "model.h"

#include <map>

#include "err.h"

void Model::load_model(const std::string& model_file_path)
{
	fs::path p = fs::path(model_file_path);
	auto abs_path = fs::canonical(p); /** get absolute path */
	directory_path = fs::path(model_file_path).parent_path().string();

	std::vector<Vertex> vertices;  /* for storing vertices */
	std::vector<uint32_t> indices; /*for storing index */
	// read file via ASSIMP
	scene = importer.ReadFile(abs_path,
	                          aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // if is Not Zero
	{
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	animation_channel_map.clear();
	for (int i = 0; i < scene->mNumAnimations; i++) {
		auto animation = scene->mAnimations[i];
		for (int j = 0; j < animation->mNumChannels; j++) {
			auto channel = animation->mChannels[j];
			animation_channel_map[std::pair<uint32_t, std::string>(i, channel->mNodeName.C_Str())] = j;
		}
	}
	//process ASSIMP's root node recursively
	processNode(scene->mRootNode, vertices, indices);
	bone_matrices.resize(bone_namer.total());
}
void Model::processNode(aiNode* node, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	// process each mesh located at the current node
	if (node->mName != aiString("objTwoHand13_SM") && node->mName != aiString("Plane001") && node->mName != aiString("Plane002") && node->mName != aiString("obj53002_LynM001")) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, vertices, indices);
		}
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	vertices.clear();
	indices.clear();
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], vertices, indices);
	}
}
void Model::processMesh(aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	auto texture_path = directory_path;
	{
		aiString material_texture_path;
		auto material = scene->mMaterials[mesh->mMaterialIndex];
		material->GetTexture(aiTextureType_DIFFUSE, 0, &material_texture_path);
		std::string item = material_texture_path.C_Str();
		int i;
		for (i = (int)item.length() - 1; i >= 0; i--)
			if (item[i] == '\\') break;
		item = item.substr(i + 1);
		texture_path = texture_path + "/" + item;
	}
	std::cout << "texture loaded: " << texture_path << std::endl;
	/* collect texture and create mesh object with this texture*/
	Vertex vertex;
	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		auto vertex = Vertex();
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		auto face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	for (int i = 0; i < mesh->mNumBones; i++) {
		auto bone = mesh->mBones[i];
		auto id = bone_namer.Name(bone->mName.C_Str());

		bone_offsets.resize(std::max(id + 1, (uint32_t)bone_offsets.size()));
		bone_offsets[id] = mat4_from_aimatrix4x4(bone->mOffsetMatrix);

		for (int j = 0; j < bone->mNumWeights; j++) {
			auto weight = bone->mWeights[j];
			vertices[weight.mVertexId].addBoneData(id, weight.mWeight);
		}
	}
	add_mesh(vertices, indices, texture_path);
}
void Model::add_mesh(std::vector<Vertex>& vertices,
                     std::vector<uint32_t>& indices,
                     const std::string& texture_path)
{
	std::shared_ptr<Mesh> mesh_ptr = Mesh::create(vertices, indices, texture_path);
	meshes.push_back(mesh_ptr);
}
glm::mat4 Model::InterpolateTranslationMatrix(aiVectorKey* keys, uint32_t n, double ticks)
{
	static auto mat4_from_aivector3d = [](aiVector3D aivector) -> glm::mat4 {
		return translate(glm::mat4(1), glm::vec3(aivector.x, aivector.y, aivector.z));
	};
	if (n == 0) return glm::mat4(1);
	if (n == 1) return mat4_from_aivector3d(keys->mValue);
	if (ticks <= keys[0].mTime) return mat4_from_aivector3d(keys[0].mValue);
	if (keys[n - 1].mTime <= ticks) return mat4_from_aivector3d(keys[n - 1].mValue);

	aiVectorKey anchor;
	anchor.mTime = ticks;
	auto right_ptr = std::upper_bound(keys, keys + n, anchor, [](const aiVectorKey& a, const aiVectorKey& b) {
		return a.mTime < b.mTime;
	});
	auto left_ptr = right_ptr - 1;

	float factor = (ticks - left_ptr->mTime) / (right_ptr->mTime - left_ptr->mTime);
	return mat4_from_aivector3d(left_ptr->mValue * (1.0f - factor) + right_ptr->mValue * factor);
}
glm::mat4 Model::InterpolateRotationMatrix(aiQuatKey* keys, uint32_t n, double ticks)
{
	static auto mat4_from_aiquaternion = [](aiQuaternion quaternion) -> glm::mat4 {
		auto rotation_matrix = quaternion.GetMatrix();
		glm::mat4 res(1);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) res[j][i] = rotation_matrix[i][j];
		return res;
	};
	if (n == 0) return glm::mat4(1);
	if (n == 1) return mat4_from_aiquaternion(keys->mValue);
	if (ticks <= keys[0].mTime) return mat4_from_aiquaternion(keys[0].mValue);
	if (keys[n - 1].mTime <= ticks) return mat4_from_aiquaternion(keys[n - 1].mValue);

	aiQuatKey anchor;
	anchor.mTime = ticks;
	auto right_ptr = std::upper_bound(keys, keys + n, anchor, [](const aiQuatKey& a, const aiQuatKey& b) {
		return a.mTime < b.mTime;
	});
	auto left_ptr = right_ptr - 1;

	double factor = (ticks - left_ptr->mTime) / (right_ptr->mTime - left_ptr->mTime);
	aiQuaternion out;
	aiQuaternion::Interpolate(out, left_ptr->mValue, right_ptr->mValue, factor);
	return mat4_from_aiquaternion(out);
}
glm::mat4 Model::InterpolateScalingMatrix(aiVectorKey* keys, uint32_t n, double ticks)
{
	static auto mat4_from_aivector3d = [](aiVector3D aivector) -> glm::mat4 {
		return scale(glm::mat4(1), glm::vec3(aivector.x, aivector.y, aivector.z));
	};
	if (n == 0) return glm::mat4(1);
	if (n == 1) return mat4_from_aivector3d(keys->mValue);
	if (ticks <= keys[0].mTime) return mat4_from_aivector3d(keys[0].mValue);
	if (keys[n - 1].mTime <= ticks) return mat4_from_aivector3d(keys[n - 1].mValue);

	aiVectorKey anchor;
	anchor.mTime = ticks;
	auto right_ptr = std::upper_bound(keys, keys + n, anchor, [](const aiVectorKey& a, const aiVectorKey& b) {
		return a.mTime < b.mTime;
	});
	auto left_ptr = right_ptr - 1;

	float factor = (ticks - left_ptr->mTime) / (right_ptr->mTime - left_ptr->mTime);
	return mat4_from_aivector3d(left_ptr->mValue * (1.0f - factor) + right_ptr->mValue * factor);
}
void Model::RecursivelyUpdateBoneMatrices(int animation_id, aiNode* node, glm::mat4 transform, double ticks)
{
	std::string node_name = node->mName.C_Str();
	auto animation = scene->mAnimations[animation_id];
	glm::mat4 current_transform;
	if (animation_channel_map.count(std::pair<uint32_t, std::string>(animation_id, node_name))) {
		uint32_t channel_id = animation_channel_map[std::pair<uint32_t, std::string>(animation_id, node_name)];
		auto channel = animation->mChannels[channel_id];

		// translation matrix
		glm::mat4 translation_matrix = InterpolateTranslationMatrix(channel->mPositionKeys, channel->mNumPositionKeys, ticks);
		// rotation matrix
		glm::mat4 rotation_matrix = InterpolateRotationMatrix(channel->mRotationKeys, channel->mNumRotationKeys, ticks);
		// scaling matrix
		glm::mat4 scaling_matrix = InterpolateScalingMatrix(channel->mScalingKeys, channel->mNumScalingKeys, ticks);

		current_transform = translation_matrix * rotation_matrix * scaling_matrix;
	}
	else {
		current_transform = mat4_from_aimatrix4x4(node->mTransformation);
	}
	if (bone_namer.map().count(node_name)) {
		uint32_t i = bone_namer.map()[node_name];
		bone_matrices[i] = transform * current_transform * bone_offsets[i];
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		RecursivelyUpdateBoneMatrices(animation_id, node->mChildren[i], transform * current_transform, ticks);
	}
}
void Model::Draw()
{
	for (const auto& mesh : meshes) {
		mesh->Draw(shader_program);
	}
}
void Model::Draw(uint32_t animation_id,
                 const std::weak_ptr<Camera>& camera_ptr,
                 const float width, const float height)
{
	using namespace glm;
	RecursivelyUpdateBoneMatrices(animation_id, scene->mRootNode, mat4(1), glfwGetTime() * scene->mAnimations[animation_id]->mTicksPerSecond);
	shader_program->use();
	glm::mat4 view = glm::rotate(mat4(1), -pi<float>() / 2.5f, vec3(1, 0, 0));
	shader_program->set_uniform("uModelMatrix", view);
	shader_program->set_uniform("uViewMatrix", camera_ptr.lock()->GetViewMatrix());
	shader_program->set_uniform("uProjectionMatrix", camera_ptr.lock()->GetProjMatrix(width, height));
	shader_program->set_uniform("uBoneMatrices", bone_matrices);
	Draw();
}
void Model::Draw(uint32_t animation_id,
                 const glm::mat4& view_matrix,
                 const glm::mat4& proj_matrix)
{
	using namespace glm;
	double time = glfwGetTime();
	RecursivelyUpdateBoneMatrices(animation_id, scene->mRootNode, mat4(1), time * scene->mAnimations[animation_id]->mTicksPerSecond);
	shader_program->use();
	glm::mat4 model_view = glm::rotate(mat4(1), glm::radians(130.0f), vec3(0.3, 0.1, 0));
	model_view = glm::scale(model_view, glm::vec3(0.05, 0.05, 0.05));
	shader_program->set_uniform("uModelMatrix", model_view);
	shader_program->set_uniform("uViewMatrix", view_matrix);
	shader_program->set_uniform("uProjectionMatrix", proj_matrix);
	shader_program->set_uniform("uBoneMatrices", bone_matrices);
	Draw();
}
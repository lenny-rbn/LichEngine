#pragma once
#include <vector>
#include <glad/glad.h>

#include <Resources/Vertex.h>
#include <Resources/Resource.h>

namespace SceneManagement
{

	struct ObjInfo;
}
enum GProperties
{
	ALPHA_TEST = GL_ALPHA_TEST,
	AUTO_NORMAL = GL_AUTO_NORMAL,
	BLEND = GL_BLEND,
	CLIP_PLANE0 = GL_CLIP_PLANE0,
	COLOR_LOGIC_OP = GL_COLOR_LOGIC_OP,
	COLOR_MATERIAL = GL_COLOR_MATERIAL,
	COLOR_SUM = GL_COLOR_SUM,
	COLOR_TABLE = GL_COLOR_TABLE,
	CONVOLUTION_1D = GL_CONVOLUTION_1D,
	CONVOLUTION_2D = GL_CONVOLUTION_2D,
	CULL_FACE = GL_CULL_FACE,
	DEPTH_TEST = GL_DEPTH_TEST,
	DITHER = GL_DITHER,
	FOG = GL_FOG,
	HISTOGRAM = GL_HISTOGRAM,
	INDEX_LOGIC_OP = GL_INDEX_LOGIC_OP,
	LIGHT0 = GL_LIGHT0,
	LIGHTING = GL_LIGHTING,
	LINE_SMOOTH = GL_LINE_SMOOTH,
	LINE_STIPPLE = GL_LINE_STIPPLE,
	MAP1_COLOR_4 = GL_MAP1_COLOR_4,
	MAP1_INDEX = GL_MAP1_INDEX,
	MAP1_NORMAL = GL_MAP1_NORMAL,
	MAP1_TEXTURE_COORD_1 = GL_MAP1_TEXTURE_COORD_1,
	MAP1_TEXTURE_COORD_2 = GL_MAP1_TEXTURE_COORD_2,
	MAP1_TEXTURE_COORD_3 = GL_MAP1_TEXTURE_COORD_3,
	MAP1_TEXTURE_COORD_4 = GL_MAP1_TEXTURE_COORD_4,
	MAP1_VERTEX_3 = GL_MAP1_VERTEX_3,
	MAP1_VERTEX_4 = GL_MAP1_VERTEX_4,
	MAP2_COLOR_4 = GL_MAP2_COLOR_4,
	MAP2_INDEX = GL_MAP2_INDEX,
	MAP2_NORMAL = GL_MAP2_NORMAL,
	MAP2_TEXTURE_COORD_1 = GL_MAP2_TEXTURE_COORD_1,
	MAP2_TEXTURE_COORD_2 = GL_MAP2_TEXTURE_COORD_2,
	MAP2_TEXTURE_COORD_3 = GL_MAP2_TEXTURE_COORD_3,
	MAP2_TEXTURE_COORD_4 = GL_MAP2_TEXTURE_COORD_4,
	MAP2_VERTEX_3 = GL_MAP2_VERTEX_3,
	MAP2_VERTEX_4 = GL_MAP2_VERTEX_4,
	MINMAX = GL_MINMAX,
	MULTISAMPLE = GL_MULTISAMPLE,
	NORMALIZE = GL_NORMALIZE,
	POINT_SMOOTH = GL_POINT_SMOOTH,
	POINT_SPRITE = GL_POINT_SPRITE,
	POLYGON_OFFSET_FILL = GL_POLYGON_OFFSET_FILL,
	POLYGON_OFFSET_LINE = GL_POLYGON_OFFSET_LINE,
	POLYGON_OFFSET_POINT = GL_POLYGON_OFFSET_POINT,
	POLYGON_SMOOTH = GL_POLYGON_SMOOTH,
	POLYGON_STIPPLE = GL_POLYGON_STIPPLE,
	POST_COLOR_MATRIX_COLOR_TABLE = GL_POST_COLOR_MATRIX_COLOR_TABLE,
	POST_CONVOLUTION_COLOR_TABLE = GL_POST_CONVOLUTION_COLOR_TABLE,
	RESCALE_NORMAL = GL_RESCALE_NORMAL,
	SAMPLE_ALPHA_TO_COVERAGE = GL_SAMPLE_ALPHA_TO_COVERAGE,
	SAMPLE_ALPHA_TO_ONE = GL_SAMPLE_ALPHA_TO_ONE,
	SAMPLE_COVERAGE = GL_SAMPLE_COVERAGE,
	SEPARABLE_2D = GL_SEPARABLE_2D,
	SCISSOR_TEST = GL_SCISSOR_TEST,
	STENCIL_TEST = GL_STENCIL_TEST,
	TEXTURE_1D = GL_TEXTURE_1D,
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_3D = GL_TEXTURE_3D,
	TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
	TEXTURE_GEN_Q = GL_TEXTURE_GEN_Q,
	TEXTURE_GEN_R = GL_TEXTURE_GEN_R,
	TEXTURE_GEN_S = GL_TEXTURE_GEN_S,
	TEXTURE_GEN_T = GL_TEXTURE_GEN_T,
	VERTEX_PROGRAM_POINT_SIZE = GL_VERTEX_PROGRAM_POINT_SIZE,
	VERTEX_PROGRAM_TWO_SIDE = GL_VERTEX_PROGRAM_TWO_SIDE,
	FRONT_LEFT = GL_FRONT_LEFT,
	FRONT_RIGHT = GL_FRONT_RIGHT,
	BACK_LEFT = GL_BACK_LEFT,
	BACK_RIGHT = GL_BACK_RIGHT,
	FRONT = GL_FRONT,
	BACK = GL_BACK,
	LEFT = GL_LEFT,
	RIGHT = GL_RIGHT,
	FRONT_AND_BACK = GL_FRONT_AND_BACK,
	NEVER = GL_NEVER,
	LESS = GL_LESS,
	EQUAL = GL_EQUAL,
	LEQUAL = GL_LEQUAL,
	GREATER = GL_GREATER,
	NOTEQUAL = GL_NOTEQUAL,
	GEQUAL = GL_GEQUAL,
	ALWAYS = GL_ALWAYS,
	ZERO = GL_ZERO,
	ONE = GL_ONE,
	SRC_COLOR = GL_SRC_COLOR,
	ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	DST_COLOR = GL_DST_COLOR,
	ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	SRC_ALPHA = GL_SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	DST_ALPHA = GL_DST_ALPHA,
	ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR = GL_CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
	SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
	SRC1_COLOR = GL_SRC1_COLOR,
	ONE_MINUS_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
	SRC1_ALPHA = GL_SRC1_ALPHA,
	ONE_MINUS_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA,
	DRAWLINE = GL_LINE,
	DRAWFILL = GL_FILL,
	DRAWPOINT = GL_POINT,
};

enum ShaderType
{
	VERTEXSHADER = GL_VERTEX_SHADER,
	FRAGMENTSHADER = GL_FRAGMENT_SHADER,
	GEOMETRYSHADER = GL_GEOMETRY_SHADER,
	NOSHADER = 0,
};

enum ResourceGraphicType
{
	TEXTURE,
	MODEL,
	SHADER,
	FRAMEBUFFER,
	SKYBOX,
};

typedef GLuint uint;
typedef GLsizei sizei;


namespace Renderer			{ class Mesh; }
namespace SceneManagement	{ class GameObject; }
namespace Physics			{ class Transform; }

namespace WrapperRHI
{
	class LICH_ENGINE RHI
	{
	private:
		static void BindTexture2D(uint texture);

		static void Draw(uint modelIndex, sizei vertexbufferSize);

		static void CreateShader(int& shaderProgram, std::vector<std::string> datas, std::vector<ShaderType> types);
		static void CreateModel(uint& VBO, uint& VAO, uint& EBO, std::vector<Vertex> vertexBuffer, std::vector<int> indexBuffer);
		static void CreateTexture(unsigned char* data, int nrChannels, int width, int height, uint& texture, uint& sampler, bool gammaCorrection);

		static void DeleteShader(int shader);
		static void DeleteTexture(uint& texture, uint& sampler);
		static void DeleteSkybox(uint& cubemap);
		static void DeleteModel(uint& VBO, uint& VAO);
		static void DeleteFramebuffers(sizei size, const uint* framebuffers);

		static uint GetUniformLocation(Resources::Resource* shader, std::string uniform);

	public:
		static void DrawMode(GProperties face, GProperties mode);
		static void UseShader(int shaderProgram);
		static void EnableOptions(GProperties toEnable);
		static void DisableOptions(GProperties toDisable);
		static void ActivateAndBindCubemap(int nbTexture, uint cubemap);
		static void ActivateAndBindTexture2D(int nbTexture, uint texture);
		static void CopyDepth(uint& fromBuffer, uint& toBuffer, const int width, const int height);

		static void SetBackgroundType();
		static void SetViewportSize(Vector2D size);
		static void SetDepthFunc(GProperties depth);
		static void SetActiveFramebuffer(uint buffer);
		static void SetCullFace(GProperties cullFace);
		static void SetViewportSize(int width, int height);
		static void SetBlendFunc(GProperties sFactor, GProperties dFactor);
		static void SetViewport(int right, int top, int width, int height);
		static void SetPixelUnpackAlignment(int alignment);

		static void Send1iToShader(Resources::Resource* shader, const char* name, int nbr);
		static void Send1fToShader(Resources::Resource* shader, const char* name, float k1);
		static void Send3fToShader(Resources::Resource* shader, const char* name, float k1, float k2, float k3);
		static void Send3fToShader(Resources::Resource* shader, const char* name, Vector3D values);
		static void Send4iToShader(Resources::Resource* shader, const char* name, Vector4D values);
		static void Send4fToShader(Resources::Resource* shader, const char* name, Vector4D values);
		static void SendMat4ToShader(Resources::Resource* shader, const char* name, GLsizei count, bool transpose, const float* value);
		static void SendMaterialToShader(Resources::Resource* shaderResource, Resources::Resource* materialResource);

		static void DrawScreenQuad(uint quadVAO);
		static void DrawMesh(SceneManagement::ObjInfo* info, Resources::Resource* shaderResource);
		static void DrawMeshForPicking(Renderer::Mesh* mesh, Physics::Transform* transform, Resources::Resource* shaderResource);
		static void DrawMeshShadowPass(SceneManagement::ObjInfo* info, Resources::Resource* shaderResource);
		static void DrawGizmos(Physics::Transform* transform, int modificationMode, Physics::Transform* cameraTransform, Resources::Resource* shaderResource);
		static void DrawGizmosForPicking(Physics::Transform* transform, int modificationMode, Physics::Transform* cameraTransform, Resources::Resource* shaderResource);
		static void DrawCollider(SceneManagement::GameObject* gameObject, Physics::Transform* cameraTransform, Resources::Resource* shaderResource);
		static void DrawSkyBox(Resources::Resource* ressourceSkybox);
		static void DrawTextUI(Resources::Resource* shaderResource, std::string text, Resources::Resource* fontResource, Vector2D pos, float scale, Vector4D color, uint& UIVAO, uint& UIVBO);
		static void DrawImageUI(uint texture, uint UIImageVAO, uint UIImageVBO);
		static void DrawMeshNotAnimated(SceneManagement::ObjInfo* info, Resources::Resource* shaderResource);

		static void CreateFrameBuffer(uint& colorBufferCount);
		static void CreateScreenQuad(uint& quadVAO, uint& quadVBO);
		static void CreateUIQuad(uint& UIVAO, uint& UIVBO);
		static void CreateUIImageQuad(uint& UIImageVAO, uint& UIImageVBO);
		static void Create(const ResourceGraphicType type, Resources::Resource* resource);
		static void CreateShadowFBOandMap(uint& shadowFBO, uint& shadowMap, Vector2D size);
		static void CreateNoiseTexture(uint &noiseTexture, std::vector<Vector3D> &ssaoNoise);
		static void CreateShadowFBOandCubeMap(uint& shadowFBO, uint& shadowCubeMap, Vector2D size);
		static void CreateHDRFrameBuffer(uint& hdrFBO, uint& rboDepth, uint (&colorBuffers)[2], int width, int height);
		static void CreatePingPongFrameBuffer(uint (&pingpongFBO)[2], uint(&pingpongBuffer)[2], int width, int height);
		static void CreateGBuffer(uint& gBuffer, uint& gPosition, uint& gViewPosition, uint& gNormal, uint& gViewNormal, uint& gColor, uint& gMetallicRoughness, uint& depthBuffer, int width, int height);
		static void CreateRenderTexture(uint& indexBuffer, uint& indexTexture, uint width, uint height);
		static void CreateOneChannelFrameBuffer(uint& frameBuffer, uint& texture, int width, int height);
		static void CreateCubeMap(std::vector<Resources::Resource*> faces, unsigned int& cubeMap);
		static void UpdateCubemapFace(int face, Resources::Resource* texture, unsigned int& cubeMap);
		static void CreateGlyph(uint& texture, int width, int height, unsigned char* data);

		static void ClearColor();
		static void ClearDepth();
		static void ClearBackground();
		static void ClearColorAndDepth();
		static void ClearColor(float r, float g, float b, float a);

		static void DeleteTexture(uint& texture);
		static void DeleteVertexArray(uint& VAO);
		static void DeleteProgram(int shaderProgram);
		static void DeleteFramebuffer(uint& frameBuffer);
		static void DeleteBuffer(uint& frameBuffer);
		static void DeleteRenderbuffer(uint& renderBuffer);
		static void Delete(const ResourceGraphicType type, Resources::Resource* resource);

		static void ReadPixelColor(float x, float y, Vector4D& data);

		static int InitOpenGL();
	};

	LAPI void LoadGLFunction();
}
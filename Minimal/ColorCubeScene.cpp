#include "ColorCubeScene.hpp"

ColorCubeScene::ColorCubeScene() : cube({ "Position", "Normal" }, oglplus::shapes::Cube()) {
	using namespace oglplus;
	try {
		// attach the shaders to the program
		prog.AttachShader(
			FragmentShader()
			.Source(GLSLSource(String(FRAGMENT_SHADER)))
			.Compile()
		);
		prog.AttachShader(
			VertexShader()
			.Source(GLSLSource(String(VERTEX_SHADER)))
			.Compile()
		);
		prog.Link();
	}
	catch (ProgramBuildError & err) {
		FAIL((const char*)err.what());
	}

	// link and use it
	prog.Use();

	vao = cube.VAOForProgram(prog);
	vao.Bind();
	// Create a cube of cubes
	{
		std::vector<mat4> instance_positions;
		for (unsigned int z = 0; z < GRID_SIZE; ++z) {
			for (unsigned int y = 0; y < GRID_SIZE; ++y) {
				for (unsigned int x = 0; x < GRID_SIZE; ++x) {
					int xpos = (x - (GRID_SIZE / 2)) * 2;
					int ypos = (y - (GRID_SIZE / 2)) * 2;
					int zpos = (z - (GRID_SIZE / 2)) * 2;
					vec3 relativePosition = vec3(xpos, ypos, zpos);
					if (relativePosition == vec3(0)) {
						continue;
					}
					instance_positions.push_back(glm::translate(glm::mat4(1.0f), relativePosition));
				}
			}
		}

		Context::Bound(Buffer::Target::Array, instances).Data(instance_positions);
		instanceCount = (GLuint)instance_positions.size();
		int stride = sizeof(mat4);
		for (int i = 0; i < 4; ++i) {
			VertexArrayAttrib instance_attr(prog, Attribute::InstanceTransform + i);
			size_t offset = sizeof(vec4) * i;
			instance_attr.Pointer(4, DataType::Float, false, stride, (void*)offset);
			instance_attr.Divisor(1);
			instance_attr.Enable();
		}
	}
}

ColorCubeScene::~ColorCubeScene()
{
}

void ColorCubeScene::render(const mat4 & projection, const mat4 & modelview) {
	using namespace oglplus;
	prog.Use();
	Uniform<mat4>(prog, "ProjectionMatrix").Set(projection);
	Uniform<mat4>(prog, "CameraMatrix").Set(modelview);
	vao.Bind();
	cube.Draw(instanceCount);
}
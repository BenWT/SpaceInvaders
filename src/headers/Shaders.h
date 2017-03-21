const GLchar *vertShaderSource[] = {
	"#version 440 core\n"
	"layout(location=0) in vec3 position;\n"
	"layout (location = 1) in vec2 texCoord;\n"
	"out vec2 TexCoord;\n"
	"uniform mat4 trans;\n"
	"uniform vec2 uvoff;\n"
	"void main()\n"
	"{\n"
		"gl_Position = trans * vec4(position.x, position.y, position.z, 1.0);\n"
		"TexCoord = vec2(texCoord.x + uvoff.x, 1.0f - texCoord.y + uvoff.y);\n"
	"}\n\0"
};

const GLchar *fragShaderSource[] = {
	"#version 440 core\n"
	"in vec2 TexCoord;\n"
	"out vec4 color;\n"
	"uniform sampler2D Texture;\n"
	"void main()\n"
	"{\n"
		"color = texture(Texture, TexCoord);\n"
	"}\n\0"
};

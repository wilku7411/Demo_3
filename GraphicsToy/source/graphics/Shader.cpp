#include "Shader.h"
#include <memory>
#include "../IO/File.h"

GLint CShader::makeShader(const std::string& source, const EShaderType type)
{
    const char* shader = source.c_str();

    GLuint shaderHandle = glCreateShader((unsigned int)type);
    glShaderSource(shaderHandle, 1, &shader, NULL);

    glCompileShader(shaderHandle);

    GLint success;
    GLint logLength;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);

    // In case of compilation errors - create compilation log.
    if (success != GL_TRUE)
    {
        std::shared_ptr<char> infoLog(new char[logLength]);
        glGetShaderInfoLog(shaderHandle, logLength, NULL, infoLog.get());
        std::cout << 
            "\nERROR::SHADER::" << (type == EShaderType::VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog.get();
        return -1;
    }

    return shaderHandle;
}

GLuint CShader::makeShaderPorgram(const GLuint& vertexShader, const GLuint& fragmentShader)
{
	GLint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

    GLint success;
    GLint logLength;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);

    // In case of linking errors - create linker log.
	if (success != GL_TRUE)
	{
        std::shared_ptr<char> infoLog(new char[logLength]);
		glGetProgramInfoLog(shaderProgram, logLength, NULL, infoLog.get());
		std::cout << "\nERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.get();
		return -1;
	}
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

	return shaderProgram;
}

/*
	Creates, compiles and link shaders
	also create shader program
*/
CShader::CShader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource)
	:
	m_programId(0),
	m_bound(false)
{
	std::string vertexSource = File::readFileContent(vertexShaderSource);
	std::string fragmentSource = File::readFileContent(fragmentShaderSource);
    
    {
        std::string temp = vertexShaderSource;
        const auto trimPoint = temp.find_last_of("/");
        m_name = temp.substr(trimPoint+1);
    }
    printf_s("\nCreated shader [%s]", m_name.c_str());

    unsigned int vertexid = makeShader(vertexSource, EShaderType::VERTEX_SHADER);
    unsigned int fragmentid = makeShader(fragmentSource, EShaderType::FRAGMENT_SHADER);

	m_programId = makeShaderPorgram(vertexid, fragmentid);
}

CShader::CShader()
    : m_programId(0)
    , m_bound(false)
{
}

CShader::~CShader()
{
    deleteShaderProgram();
}

void CShader::use()
{
	m_bound = true;
	glUseProgram(m_programId);
}

void CShader::disable()
{
	m_bound = false;
	glUseProgram(0);
}

void CShader::reset(const GLchar* newVertexShaderSource, const GLchar* newFragmentShaderSource)
{
    std::string vertexSource = File::readFileContent(newVertexShaderSource);
    std::string fragmentSource = File::readFileContent(newFragmentShaderSource);

    int vertexid = makeShader(vertexSource, EShaderType::VERTEX_SHADER);
    int fragmentid = makeShader(fragmentSource, EShaderType::FRAGMENT_SHADER);

    unsigned int tempID = makeShaderPorgram(vertexid, fragmentid);

    std::string temp = newVertexShaderSource;
    const auto trimPoint = temp.find_last_of("/");
    temp = temp.substr(trimPoint+1);

    if(vertexid > 0)
    {
        deleteShaderProgram();
        m_programId = tempID;
        m_name = temp;
        this->m_bound = false;

        printf_s("\nChanged shader [%s] to [%s]", m_name.c_str(), temp.c_str());
    }
    else
    {
        glDeleteProgram(tempID);
        printf_s("\nFailed to change shader [%s] to [%s]", m_name.c_str(), temp.c_str());
    }
}

void CShader::deleteShaderProgram()
{
    glDeleteProgram(m_programId);
}

GLuint CShader::getProgramID() const
{
	return m_programId;
}

std::string CShader::getName() const 
{
    return m_name;
}


void CShader::setBool(const unsigned int uniformLocation, bool value)
{
	if (!m_bound)
		use();
	glUniform1i(uniformLocation, static_cast<bool>(value));
	disable();
}

void CShader::setInt(const unsigned int uniformLocation, int value)
{
	if (!m_bound)
		use();
	glUniform1i(uniformLocation, value);
	disable();
}

void CShader::setFloat(const unsigned int uniformLocation, float value)
{
	if (!m_bound)
		use();
	glUniform1f(uniformLocation, value);
	disable();
}

void CShader::setVec2(const unsigned int uniformLocation, const glm::vec2 &value)
{
	if (!m_bound)
		use();
	glUniform3fv(uniformLocation, 1, &value[0]);
	disable();
}

void CShader::setVec2(const unsigned int uniformLocation, float x, float y)
{
	if (!m_bound)
		use();
	glUniform2f(uniformLocation, x, y);
	disable();
}

void CShader::setVec3(const unsigned int uniformLocation, const glm::vec3 &value)
{
	if (!m_bound)
		use();
	glUniform3fv(uniformLocation, 1, &value[0]);
	disable();
}

void CShader::setVec3(const unsigned int uniformLocation, float x, float y, float z)
{
	if (!m_bound)
		use();
	glUniform3f(uniformLocation, x, y, z);
	disable();
}

void CShader::setVec4(const unsigned int uniformLocation, const glm::vec4 &value)
{
	if (!m_bound)
		use();
	glUniform4fv(uniformLocation, 1, &value[0]);
	disable();
}

void CShader::setVec4(const unsigned int uniformLocation, float x, float y, float z, float w)
{
	if (!m_bound)
		use();
	glUniform4f(uniformLocation, x, y, z, w);
	disable();
}

void CShader::setMat2(const unsigned int uniformLocation, const glm::mat2 &mat)
{
	if (!m_bound)
		use();
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	disable();
}

void CShader::setMat3(const unsigned int uniformLocation, const glm::mat3 &mat)
{
	if (!m_bound)
		use();
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	disable();
}

void CShader::setMat4(const unsigned int uniformLocation, const glm::mat4 &mat)
{
	if (!m_bound)
		use();
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	disable();
}


void CShader::setBool(const std::string& uniformName, bool value)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setBool(uniformLocation, value);
}

void CShader::setInt(const std::string& uniformName, int value)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setInt(uniformLocation, value);
}

void CShader::setFloat(const std::string& uniformName, float value)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setFloat(uniformLocation, value);
}

void CShader::setVec2(const std::string& uniformName, const glm::vec2 &value)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setVec2(uniformLocation, value);
}

void CShader::setVec2(const std::string& uniformName, float x, float y)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setVec2(uniformLocation, x, y);
}

void CShader::setVec3(const std::string& uniformName, const glm::vec3 &value)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setVec3(uniformLocation, value);
}

void CShader::setVec3(const std::string& uniformName, float x, float y, float z)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setVec3(uniformLocation, x, y, z);
}

void CShader::setVec4(const std::string& uniformName, const glm::vec4 &value)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setVec4(uniformLocation, value);
}

void CShader::setVec4(const std::string& uniformName, float x, float y, float z, float w)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setVec4(uniformLocation, x, y, z, w);
}

void CShader::setMat2(const std::string& uniformName, const glm::mat2 &mat)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setMat2(uniformLocation, mat);
}

void CShader::setMat3(const std::string& uniformName, const glm::mat3 &mat)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setMat3(uniformLocation, mat);
}

void CShader::setMat4(const std::string& uniformName, const glm::mat4 &mat)
{
	auto uniformLocation = glGetUniformLocation(m_programId, uniformName.c_str());
	setMat4(uniformLocation, mat);
}

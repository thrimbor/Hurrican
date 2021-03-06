/**
 *
 *  Copyright (C) 2011-2015 Scott R. Smith
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 */

#include "cshader.hpp"
#include "SDL_port.hpp"

CShader::CShader()
    : NamePos(GL_INVALID_VALUE),
      NameClr(GL_INVALID_VALUE),
      NameTex(GL_INVALID_VALUE),
      NameMvp(GL_INVALID_VALUE),
      Program(GL_INVALID_VALUE),
      Shaders(),
      Uniforms(),
      Attributes() {}

void CShader::Close() {
    uint16_t i;

    for (i = 0; i < Shaders.size(); i++) {
        if (Shaders.at(i).name != GL_INVALID_VALUE) {
            glDeleteShader(Shaders.at(i).name);
        }
    }

    if (Program != GL_INVALID_VALUE) {
        glDeleteProgram(Program);
    }

    Program = GL_INVALID_VALUE;
    Shaders.clear();
}

int8_t CShader::Load(const std::string &path_vertex, const std::string &path_frag) {
    if (LoadShader(GL_VERTEX_SHADER, path_vertex))
        return 1;

    if (LoadShader(GL_FRAGMENT_SHADER, path_frag))
        return 1;

    if (CreateProgram())
        return 1;

    return 0;
}

int8_t CShader::LoadShader(GLenum type, const std::string &path) {
    shader_t shader;

    Protokoll << "Shader: Compiling " << path << std::endl;

    shader.type = type;
    shader.path = path;
    shader.name = CompileShader(shader.type, shader.path);

    if (shader.name == GL_INVALID_VALUE) {
        return 1;
    } else {
        Protokoll << "Shader: Compiled " << path << std::endl;
    }

    Shaders.push_back(shader);

    return 0;
}

void CShader::Use() {
    glUseProgram(Program);
}

GLuint CShader::CompileShader(GLenum type, const std::string &path) {
    GLint status;
    GLuint shader;
    std::string shadertype;
    std::vector<char> source;
    uint32_t size = 0;

    source = LoadFileToMemory(path);

#if defined(USE_GLES2)
    const std::string version = "#version 100\n";
    const std::string precision = "precision mediump float;\n";
    source.insert(source.begin(), precision.begin(), precision.end());
    source.insert(source.begin(), version.begin(), version.end());
#elif defined(USE_GLES3)
    const std::string version = "#version 320 es\n";
    const std::string precision = "precision highp float;\n";
    source.insert(source.begin(), precision.begin(), precision.end());
    source.insert(source.begin(), version.begin(), version.end());
#endif

    if (!source.empty()) {
#if defined(DEBUG)
        Protokoll << "Shader Source Begin\n" << source << "\nShader Source End" << std::endl;
#endif

        shader = glCreateShader(type);
        const GLchar *shader_src = source.data();
        glShaderSource(shader, 1, &shader_src, nullptr);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            switch (type) {
                case GL_VERTEX_SHADER:
                    shadertype = "vertex";
                    break;
                case GL_FRAGMENT_SHADER:
                    shadertype = "fragment";
                    break;
                default:
                    shadertype = "unknown";
                    break;
            }

            Protokoll << "ERROR Shader: Compile failure in " << shadertype << " shader: " << path << std::endl;
            PrintLog(SHADER, shader);
            return GL_INVALID_VALUE;
        }
    } else {
        Protokoll << "ERROR Shader: File read failure in " << shadertype << " shader: " << path << std::endl;
        return GL_INVALID_VALUE;
    }

    return shader;
}

int8_t CShader::CreateProgram() {
    uint16_t i;
    GLint status;

    Program = glCreateProgram();

    for (i = 0; i < Shaders.size(); i++) {
        glAttachShader(Program, Shaders[i].name);
    }

    glLinkProgram(Program);

    glGetProgramiv(Program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        Protokoll << "ERROR Shader: Linker failure" << std::endl;
        PrintLog(PROGRAM);
        Program = GL_INVALID_VALUE;
        return 1;
    }

    FindAttributes();
    FindUniforms();

    return 0;
}

void CShader::FindAttributes() {
    GLint numAttributes;
    GLint maxAttributeLen;
    char *attributeName;

    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);
    attributeName = new char[maxAttributeLen];

    for (GLint index(0); index < numAttributes; ++index) {
        GLint size;
        GLenum type;
        GLint location;

        glGetActiveAttrib(Program, index, maxAttributeLen, nullptr, &size, &type, attributeName);
        location = glGetAttribLocation(Program, attributeName);

        std::pair<std::string, GLint> parameter;
        parameter.first = std::string(attributeName);
        parameter.second = location;
        Attributes.push_back(parameter);
    }

    delete[] attributeName;
}

void CShader::FindUniforms() {
    GLint numUniforms;
    GLint maxUniformLen;
    char *uniformName;

    glGetProgramiv(Program, GL_ACTIVE_UNIFORMS, &numUniforms);
    glGetProgramiv(Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
    uniformName = new char[maxUniformLen];

    for (GLint index(0); index < numUniforms; ++index) {
        GLint size;
        GLenum type;
        GLint location;

        glGetActiveUniform(Program, index, maxUniformLen, nullptr, &size, &type, uniformName);
        location = glGetUniformLocation(Program, uniformName);

        std::pair<std::string, GLint> parameter;
        parameter.first = std::string(uniformName);
        parameter.second = location;
        Uniforms.push_back(parameter);
    }

    delete[] uniformName;
}

GLint CShader::GetAttribute(const std::string &attribute) {
    for (std::vector<std::pair<std::string, GLint> >::const_iterator itr(Attributes.begin()); itr < Attributes.end();
         ++itr) {
        if (attribute == itr->first)
            return itr->second;
    }

    Protokoll << "ERROR Shader: Could not locate attribute: " << attribute << std::endl;

    return GL_INVALID_VALUE;
}

GLint CShader::GetUniform(const std::string &uniform) {
    for (std::vector<std::pair<std::string, GLint> >::const_iterator itr(Uniforms.begin()); itr < Uniforms.end();
         ++itr) {
        if (uniform == itr->first)
            return itr->second;
    }

    Protokoll << "ERROR Shader: Could not locate uniform: " << uniform << std::endl;

    return GL_INVALID_VALUE;
}

void CShader::PrintLog(uint8_t type, GLuint shader) {
    GLint loglength;
    char *log;

    if (CHECK_FLAG(type, SHADER)) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);

        log = new char[loglength + 1];
        glGetShaderInfoLog(shader, loglength, nullptr, log);
        Protokoll << "Shader: Shader log:\n " << log << std::endl;
        delete[] log;
    }

    if (CHECK_FLAG(type, PROGRAM)) {
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &loglength);

        log = new char[loglength + 1];
        glGetProgramInfoLog(Program, loglength, nullptr, log);
        Protokoll << "Shader: Program log:\n " << log << std::endl;
        delete[] log;
    }
}

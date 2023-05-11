#ifndef CG_ENGINE_SHADER_H
#define CG_ENGINE_SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <CG_Engine_Core/math/vec2.h>
#include <CG_Engine_Core/math/vec3.h>
#include <CG_Engine_Core/math/vec4.h>
#include <CG_Engine_Core/math/mat2x2.h>
#include <CG_Engine_Core/math/mat3x3.h>
#include <CG_Engine_Core/math/mat4x4.h>
#include "assimp/color4.h"

// T

class Shader {
public:
    // program ID
    unsigned int id;

    /*
        constructors
    */

    // default
    Shader() = default;

    // initialize with paths to vertex and fragment shaders
    Shader(const char *vertexShaderPath, const char *fragShaderPath) {
        generate(vertexShaderPath, fragShaderPath);
    };

    /*
        process functions
    */

    // generate using vertex and frag shaders
    void generate(const char *vertexShaderPath, const char *fragShaderPath) {
        int success;
        char infoLog[512];

        // compile shaders
        GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
        GLuint fragShader = compileShader(fragShaderPath, GL_FRAGMENT_SHADER);

        // create program and attach shaders
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragShader);
        glLinkProgram(id);

        // linking errors
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "Linking error:" << std::endl << infoLog << std::endl;
        }

        // delete shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
    };

    // activate shader
    void activate() {
        glUseProgram(id);
    };

    /*
        utility functions
    */

    // load string from file
    std::string loadShaderSrc(const char *filePath) {
        std::ifstream file;
        std::stringstream buf;

        std::string ret;

        // open file
        file.open(filePath);

        if (file.is_open()) {
            // read buffer
            buf << file.rdbuf();
            ret = buf.str();
        } else {
            std::cout << "Could not open " << filePath << std::endl;
        }

        // close file
        file.close();

        return ret;
    };

    // compile shader program
    GLuint compileShader(const char *filePath, GLuint type) {
        int success;
        char infoLog[512];

        // create shader from file
        GLuint ret = glCreateShader(type);
        std::string shaderSrc = loadShaderSrc(filePath);
        const GLchar *shader = shaderSrc.c_str();
        glShaderSource(ret, 1, &shader, NULL);
        glCompileShader(ret);

        // catch compilation error
        glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(ret, 512, NULL, infoLog);
            std::cout << "Error with shader comp.:" << std::endl << infoLog << std::endl;
        }

        return ret;
    };

    /*
        set uniform variables
    */

    void setBool(const std::string &name, bool value) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void set3Float(const std::string &name, float v1, float v2, float v3) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
    }

    void set3Float(const std::string &name, Vec3 v) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
    }

    void set4Float(const std::string &name, float v1, float v2, float v3, float v4) {
        glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
    }

    void set4Float(const std::string &name, aiColor4D color) {
        glUniform4f(glGetUniformLocation(id, name.c_str()), color.r, color.g, color.b, color.a);
    }

    void set4Float(const std::string &name, Vec4 v) {
        glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
    }

    void setMat4(const std::string &name, Mat4x4 val) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &val[0][0]);
    }
};

#endif

#ifndef CG_ENGINE_SHADER_H
#define CG_ENGINE_SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vec2.h>
#include <vec3.h>
#include <vec4.h>
#include <mat2x2.h>
#include <mat3x3.h>
#include <mat4x4.h>

const int BUFF_SIZE = 512;

class Shader {
private:
    std::string vCode, fCode;
    std::ifstream vShaderFile, fShaderFile;
    std::stringstream vShaderStream, fShaderStream;
    const char *vShaderData, *fShaderData;
    unsigned int vertex, fragment;
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath) {
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vCode = vShaderStream.str();
            fCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "Failed file read\n";
            return;
        }

        vShaderData = vCode.c_str();
        fShaderData = fCode.c_str();

        this->vertex = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertex, 1, &vShaderData, nullptr);
        glCompileShader(vertex);

        int success;
        char infoLog[BUFF_SIZE];

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(vertex, BUFF_SIZE, nullptr, infoLog);
            std::cout << "Error shader vertex: " << infoLog << std::endl;
        }

        // fragment
        this->fragment = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragment, 1, &fShaderData, nullptr);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(fragment, BUFF_SIZE, nullptr, infoLog);
            std::cout << "Error shader fragment:" << infoLog << std::endl;
        }

        // Program
        this->ID = glCreateProgram();

        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(ID, BUFF_SIZE, nullptr, infoLog);
            std::cout << "Error shader program: " << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    void use() const {
        glUseProgram(this->ID);
    };

    void setBool(const std::string &name, bool val) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) val);
    };

    void setInt(const std::string &name, int val) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
    };

    void setFloat(const std::string &name, float val) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
    };

    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
    };

    void setVec2(const std::string &name, const Vec2 &value) const {
        glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value.x);
    };

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
    };

    void setVec3(const std::string &name, const Vec3 &value) const {
        glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value.x);
    };

    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
    };

    void setVec4(const std::string &name, const Vec4 &value) const {
        glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value.x);
    };

    void setMat2x2(const std::string &name, const Mat2x2 &value) const {
        glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value.matrix[0][0]);
    }

    void setMat3x3(const std::string &name, const Mat3x3 &value) const {
        glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value.matrix[0][0]);
    };

    void setMat4x4(const std::string &name, const Mat4x4 &value) const {
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value.matrix[0][0]);
    }
};

#endif

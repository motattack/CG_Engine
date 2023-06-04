#ifndef CG_ENGINE_SHADER_H
#define CG_ENGINE_SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <CG_Engine/math/common.h>
#include "assimp/color4.h"

class Shader {
private:
    unsigned int programID;

    void checkCompileErrors(unsigned int shader, const std::string &type) {
        int success;
        char infoLog[512];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cout << "ERROR:SHADER::" << type << std::endl << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 512, NULL, infoLog);
                std::cout << "ERROR::LINKING::" << type << std::endl << infoLog << std::endl;
            }
        }
    };

public:
    unsigned int ID() const {
        return programID;
    };

    Shader(const char *vertexPath, const char *fragmentPath) {
        const char *vShaderCode, *fShaderCode;

        std::string vTmpStr, fTmpStr;

        std::ifstream vShaderFile, fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            std::stringstream vShaderStream;
            vShaderFile.open(vertexPath);
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vTmpStr = vShaderStream.str();
            vShaderCode = vTmpStr.c_str();

            std::stringstream fShaderStream;
            fShaderFile.open(fragmentPath);
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fTmpStr = fShaderStream.str();
            fShaderCode = fTmpStr.c_str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        unsigned int vertex;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        unsigned int fragment;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        programID = glCreateProgram();
        glAttachShader(programID, vertex);
        glAttachShader(programID, fragment);
        glLinkProgram(programID);
        checkCompileErrors(programID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    ~Shader() {
        glDeleteProgram(programID);
    };

    void use() const {
        glUseProgram(programID);
    };

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    }

    void set3Float(const std::string &name, float v1, float v2, float v3) const {
        glUniform3f(glGetUniformLocation(programID, name.c_str()), v1, v2, v3);
    }

    void set3Float(const std::string &name, Vec3 v) const {
        glUniform3f(glGetUniformLocation(programID, name.c_str()), v.x, v.y, v.z);
    }

    void set4Float(const std::string &name, Mat4x4 val) const {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
    }

    void set4Float(const std::string &name, float v1, float v2, float v3, float v4) const {
        glUniform4f(glGetUniformLocation(programID, name.c_str()), v1, v2, v3, v4);
    }

    void set4Float(const std::string &name, aiColor4D color) const {
        glUniform4f(glGetUniformLocation(programID, name.c_str()), color.r, color.g, color.b, color.a);
    }

    void set4Float(const std::string &name, Vec4 v) const {
        glUniform4f(glGetUniformLocation(programID, name.c_str()), v.x, v.y, v.z, v.w);
    }

    void setMat4(const std::string &name, Mat4x4 val) const {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
    }
};

#endif

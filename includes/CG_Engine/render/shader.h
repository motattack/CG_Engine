#ifndef CG_ENGINE_SHADER_H
#define CG_ENGINE_SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "assimp/color4.h"

#include <CG_Engine/math/common.h>

class Shader {
private:
    unsigned int programID;
public:
    unsigned int program() const {
        return programID;
    };

    Shader() : programID(0) {}

    Shader(const GLuint id) : programID(id) {}

    ~Shader() {
        glDeleteProgram(programID);
    };

    void bind() const {
        glUseProgram(programID);
    };

    void unBind() {
        glUseProgram(0);
    }

    Shader &operator=(Shader &&other) {
        if (this != &other) {
            // Move assignment implementation to transfer resources efficiently
            std::swap(programID, other.programID);
        }
        return *this;
    }

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

    void setV3Float(const std::string &name, Vec3 v) const {
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

    void setCube(const GLchar *uName, GLuint cube, GLint unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cube);
        glUniform1i(glGetUniformLocation(programID, uName), unit);
    }

    void SetSample2D(const GLchar *uName, GLuint tex2d, GLint unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, tex2d);
        glUniform1i(glGetUniformLocation(programID, uName), unit);
    }
};

#endif

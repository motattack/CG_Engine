#ifndef CG_ENGINE_SHADER_H
#define CG_ENGINE_SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "vec3.h"

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

        glShaderSource(vertex, 1, &vShaderData, NULL);
        glCompileShader(vertex);

        int success;
        char infoLog[BUFF_SIZE];

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(vertex, BUFF_SIZE, NULL, infoLog);
            std::cout << "Error shader vertex: " << infoLog << std::endl;
        }

        // fragment
        this->fragment = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragment, 1, &fShaderData, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(fragment, BUFF_SIZE, NULL, infoLog);
            std::cout << "Error shader fragment:" << infoLog << std::endl;
        }

        // Program
        this->ID = glCreateProgram();

        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(ID, BUFF_SIZE, NULL, infoLog);
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

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
    };

    void setVec3(const std::string &name, const Vec3 &value) const {
        glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value.x);
    };
};

#endif

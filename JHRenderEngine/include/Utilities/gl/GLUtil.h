//
//  GLUtil.h
//
//  Created by Jingshu Huang on 8/28/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __GLUtil__
#define __GLUtil__

#include <string>
#include "platformGL.h"

typedef enum {
	BlendNone,
	BlendAlpha,
	BlendAdditive,
    BlendAlphaAdditive,
    BlendCustom
} BlendMode;

typedef enum {
	CullNone,
	CullBack,
	CullFront,
	CullFrontAndBack
} CullMode;

// utilility
bool glGetErrorPrettyPrint(const int lineNumber);

void GL_init();
void GL_clearColor(float r, float g, float b, float a);
void GL_clear(GLbitfield clearBuffers);
void GL_viewport(GLint x, GLint y, GLsizei width, GLsizei height);

// vbo
void GL_genBuffers(GLsizei number, GLuint *buffers);
void GL_deleteBuffers(GLsizei number, const GLuint *buffers);
void GL_bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void GL_bindBuffer(GLenum type, GLuint bufferId);
void GL_unbindBuffer();
void GL_verifyBuffer(GLuint bufferId);


// Rendering state.
void GL_pixelStorei(GLenum pname, GLint param);
void GL_setZTest(bool state);
void GL_setZWrite(bool state);
void GL_setBackfaceCulling(bool state);
void GL_setFaceCulling(CullMode mode);
void GL_setBlending(bool state);
void GL_setBlendFunc(int src, int dst);


// Texture.
void GL_genTextures(GLenum type, GLuint *texId);
void GL_bindTexture(GLenum type, GLuint texId);
void GL_compressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data);
void GL_texImage2D(GLenum target, int level, int internalFormat, GLsizei w, GLsizei h, int border, GLenum format, GLenum type, GLvoid *data);
void GL_texParameteri(GLenum target, GLenum pname, GLint param);
void GL_activeTexture(GLenum texture);


// Shader attributes and uniforms
void GL_uniform1fv(GLint location, GLsizei count, const GLfloat *value);
void GL_uniform2fv(GLint location, GLsizei count, const GLfloat *value);
void GL_uniform3fv(GLint location, GLsizei count, const GLfloat *value);
void GL_uniform4fv(GLint location, GLsizei count, const GLfloat *value);
void GL_uniform1iv(GLint location, GLsizei count, const GLint *value);
void GL_uniform2iv(GLint location, GLsizei count, const GLint *value);
void GL_uniform3iv(GLint location, GLsizei count, const GLint *value);
void GL_uniform4iv(GLint location, GLsizei count, const GLint *value);
void GL_uniform1i(GLint location, GLint value);
void GL_uniform1f(GLint location, GLfloat value);
void GL_uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

void GL_enableVertexAttribArray(GLuint location);
void GL_disableVertexAttribArray(GLuint location);
void GL_vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

GLint GL_getUniformLocation(GLuint program, const GLchar *name);
GLint GL_getAttribLocation(GLuint program, const GLchar *name);


// Shader program compile, link
GLuint GL_createShader(GLenum shaderType);
void GL_deleteShader(GLuint shader);
void GL_shaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
bool GL_compileShader(GLuint shader);
GLuint GL_createProgram();
void GL_deleteProgram(GLuint& program);
void GL_attachShader(GLuint program, GLuint shader);
bool GL_linkProgram(GLuint program);
bool GL_validateProgram(GLuint program);
void GL_useProgram(GLuint program);


// draw calls.
void GL_drawArrays(GLenum mode, GLint first, GLsizei count);
void GL_drawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);

#endif

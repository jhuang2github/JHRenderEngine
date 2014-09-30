//
//  GLUtil.cpp
//
//  Created by Jingshu Huang on 8/28/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "GLUtil.h"
#include "platformDef.h"


#define GLGLUE_DEBUG


#ifdef GLGLUE_DEBUG
bool glGetErrorPrettyPrint(const int lineNumber) {
    bool hasError = false;

	int error = glGetError();
	while (error != GL_NO_ERROR) {  // Make sure we clean up the errors.
		switch (error) {
			case GL_INVALID_ENUM:
                LOGD("\nglError: GL_INVALID_ENUM! Line: %d, File: %s\n", lineNumber, __FILENAME__);
                break;
			case GL_INVALID_VALUE:
                LOGD("\nglError: GL_INVALID_VALUE! Line: %d, File: %s\n", lineNumber, __FILENAME__);
                break;
			case GL_INVALID_OPERATION:
                LOGD("\nglError: GL_INVALID_OPERATION! Line: %d\n", lineNumber);
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                LOGD("\nglError: GL_INVALID_FRAMEBUFFER_OPERATION! Line: %d\n", lineNumber);
                break;
			case GL_OUT_OF_MEMORY:
                LOGD("\nglError: GL_OUT_OF_MEMORY! Line: %d\n", lineNumber);
                break;
			case GL_STACK_UNDERFLOW:
                LOGD("\nglError: GL_STACK_UNDERFLOW! Line: %d\n", lineNumber);
                break;
			case GL_STACK_OVERFLOW:
                LOGD("\nglError: GL_STACK_OVERFLOW! Line: %d\n", lineNumber);
                break;
            default:
                LOGD("\nglError: uncategorised: %d, err = %d\n", lineNumber, error);
                break;
		}
        error = glGetError();
        hasError = true;
	}

    return hasError;
}

#else
#define 
#endif


void GL_init() {
	glGetErrorPrettyPrint(__LINE__);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

    // ES 1.0
//	glDisable(GL_TEXTURE_2D);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glGetErrorPrettyPrint(__LINE__);
}

void GL_clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void GL_clear(GLbitfield clearBuffers) {
    glClear(clearBuffers);
}


void GL_viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    glViewport(x, y, width, height);
}

#pragma rendering state related.

void GL_setZTest(bool state) {
	static bool lastZTest = false;
	if (state == lastZTest) {
		return;
	}
	lastZTest = state;
	state ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_setZWrite(bool state) {
	static bool lastZWrite = false;
	if (state == lastZWrite) {
		return;
	}
	lastZWrite = state;
	glDepthMask(state);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_setBackfaceCulling(bool state) { // TODO: need culling type!
	static bool lastBackfaceCulling = false;
	if (state == lastBackfaceCulling) {
		return;
	}
	lastBackfaceCulling = state;
	state ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_setFaceCulling(CullMode mode) {
	static CullMode lastCullingMode = CullNone;
	if (mode == lastCullingMode) {
		return;
	}
	if (mode == CullNone) {
		glEnable(GL_CULL_FACE);
	}
	switch (mode) {
		case CullNone:
			glDisable(GL_CULL_FACE);
			break;
		case CullBack:
			glCullFace(GL_BACK);
			break;
		case CullFront:
			glCullFace(GL_FRONT);
			break;
		case CullFrontAndBack:
			glCullFace(GL_FRONT_AND_BACK);
			break;
		default:
			LOGE(false, "GLGlue::setFaceCulling failed: wrong value!");
			break;
	}
	lastCullingMode = mode;
	glGetErrorPrettyPrint(__LINE__);
}

void GL_setBlending(bool state) {
	static bool lastBlending = false;
	if (state == lastBlending) {
		return;
	}
	lastBlending = state;
	state ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_setBlendFunc(int src, int dst) {
	static int lastBlendSrc = 0;
	static int lastBlendDst = 0;
    //	int src_alpha = GL_SRC_ALPHA;
    //	int one_minus_src_alpha = GL_ONE_MINUS_SRC_ALPHA;
    //	int one = GL_ONE;
    //	printf("%d, %d, %d\n", src_alpha, one_minus_src_alpha, one);
	if (src == lastBlendSrc && dst == lastBlendDst) {
		return;
	}
	lastBlendSrc = src;
	lastBlendDst = dst;
	glBlendFunc(src, dst);
	glGetErrorPrettyPrint(__LINE__);
}


#pragma draw calls

void GL_drawArrays(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
    glGetErrorPrettyPrint(__LINE__);
}

void GL_drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {
    glDrawElements(mode, count, type, indices);
    glGetErrorPrettyPrint(__LINE__);
}

#pragma vbo related.

//static GLint lastBoundVBO[2] = {0, 0};

void GL_genBuffers(GLsizei number, GLuint *buffers) {
	glGenBuffers(number, buffers);
    LOGD("tid=%d [GLUtil glGenBuffers %d", getThreadID(), *buffers);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_deleteBuffers(GLsizei number, const GLuint *buffers) {
	glDeleteBuffers(number, buffers);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
	glBufferData(target, size, data, usage);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_bindBuffer(GLenum type, GLuint bufferId) {
	glGetErrorPrettyPrint(__LINE__);
#ifdef GLGLUE_DEBUG
    GL_verifyBuffer(bufferId);
#endif
//	int index = type - GL_ARRAY_BUFFER;
//	if (lastBoundVBO[index] != bufferId) {
		glBindBuffer(type, bufferId);
//		lastBoundVBO[index] = bufferId;
//	}
	glGetErrorPrettyPrint(__LINE__);
}

void GL_unbindBuffer() {
//	if (lastBoundVBO[0] != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		lastBoundVBO[0] = 0;
//	}
//	if (lastBoundVBO[1] != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//		lastBoundVBO[1] = 0;
//	}
	glGetErrorPrettyPrint(__LINE__);
}

void GL_verifyBuffer(GLuint bufferId) {
	if (!glIsBuffer(bufferId)) {
//		LOGD("Error: GL_verifyBuffer failed: buffer id is not point to valid buffer on gpu!");
	}
}


#pragma texture related.

void GL_bindTexture(GLenum type, GLuint texId) {
	static GLuint lastTexture = 0;
	if (texId == 0 || lastTexture == texId) {
		return;
	}
	lastTexture = texId;
	glBindTexture(type, texId);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_genTextures(GLenum type, GLuint *texId) {
	glGenTextures(type, texId);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_compressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data) {

    glCompressedTexImage2D(target, level, internalformat,
                           width, height, border, imageSize, data);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_texImage2D(GLenum target, int level, int internalFormat, GLsizei w, GLsizei h, int border, GLenum format, GLenum type, GLvoid *data) {
	glTexImage2D(target, level, internalFormat, w, h, border, format, type, data);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_texParameteri(GLenum target, GLenum pname, GLint param) {
	glTexParameteri(target, pname, param);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_pixelStorei(GLenum pname, GLint param) {
	glPixelStorei(pname, param);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_activeTexture(GLenum texture) {
	glActiveTexture(texture);
	glGetErrorPrettyPrint(__LINE__);
}


#pragma shader uniform, attributes related.

void GL_uniform1fv(GLint location, GLsizei count, const GLfloat *value) {
	glGetErrorPrettyPrint(__LINE__);
	glUniform1fv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	glGetErrorPrettyPrint(__LINE__);
	glUniform2fv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	glGetErrorPrettyPrint(__LINE__);
	glUniform3fv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	glGetErrorPrettyPrint(__LINE__);
	glUniform4fv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform1iv(GLint location, GLsizei count, const GLint *value) {
	glUniform1iv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform2iv(GLint location, GLsizei count, const GLint *value) {
	glUniform2iv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform3iv(GLint location, GLsizei count, const GLint *value) {
	glUniform3iv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform41iv(GLint location, GLsizei count, const GLint *value) {
	glUniform4iv(location, count, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform1i(GLint location, GLint value) {
	glUniform1i(location, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniform1f(GLint location, GLfloat value) {
	glUniform1f(location, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glGetErrorPrettyPrint(__LINE__);
	glUniformMatrix4fv(location, count, transpose, value);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_enableVertexAttribArray(GLuint location) {
	glEnableVertexAttribArray(location);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_disableVertexAttribArray(GLuint location) {
	glDisableVertexAttribArray(location);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glGetErrorPrettyPrint(__LINE__);
}


#pragma shader compile and link related.

static bool displayProgramInfoLog(const GLuint prog, const string& info) {
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        LOGD("%s: \n%s", info.c_str(), log);
        free(log);
        return false;
    }
    return true;
}

static bool displayShaderInfoLog(const GLuint shader, const string& info) {
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        LOGD("Shader compile log:\n%s", log);
        free(log);
        return false;
    }
    return true;
}


GLuint GL_createShader(GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	glGetErrorPrettyPrint(__LINE__);
	return shader;
}

void GL_deleteShader(GLuint shader) {
	glDeleteShader(shader);
	glGetErrorPrettyPrint(__LINE__);
}

void GL_shaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) {
	glShaderSource(shader, count, string, length);
	glGetErrorPrettyPrint(__LINE__);
}

bool GL_compileShader(GLuint shader) {
	glCompileShader(shader);
	glGetErrorPrettyPrint(__LINE__);
#ifdef GLGLUE_DEBUG
	GLint compileSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
	if (compileSuccess != GL_TRUE) {
        displayShaderInfoLog(shader, "--- Shader compile log ---");
        GL_deleteShader(shader);
        LOGD("tid=%d [GLUtil compileShader] failed compiled shader.\n", getThreadID());
		return false;
	}
#endif
	glGetErrorPrettyPrint(__LINE__);
	return true;
}

GLuint GL_createProgram() {
	GLuint program = glCreateProgram();
	glGetErrorPrettyPrint(__LINE__);
	return program;
}

void GL_deleteProgram(GLuint& program) {
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
	glGetErrorPrettyPrint(__LINE__);
}

void GL_attachShader(GLuint program, GLuint shader) {
	glAttachShader(program, shader);
	glGetErrorPrettyPrint(__LINE__);
}

bool GL_linkProgram(GLuint program) {
	glLinkProgram(program);
	glGetErrorPrettyPrint(__LINE__);
#ifdef GLGLUE_DEBUG
	GLint linkSuccess;
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess != GL_TRUE) {
        displayProgramInfoLog(program, "Program link failed. link log");
		return false;
	}
#endif
	glGetErrorPrettyPrint(__LINE__);
	return true;
}

bool GL_validateProgram(GLuint program) {
    assert(program > 0);
    glValidateProgram(program);
    displayProgramInfoLog(program, "Program validate log");
    
    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        glGetErrorPrettyPrint(__LINE__);
        return false;
	}
	glGetErrorPrettyPrint(__LINE__);
    return true;
}

GLint GL_getUniformLocation(GLuint program, const GLchar *name) {
	glGetErrorPrettyPrint(__LINE__);
	GLint location = glGetUniformLocation(program, name);
	glGetErrorPrettyPrint(__LINE__);
	return location;
}

GLint GL_getAttribLocation(GLuint program, const GLchar *name) {
	glGetErrorPrettyPrint(__LINE__);
	GLint location = glGetAttribLocation(program, name);
	glGetErrorPrettyPrint(__LINE__);
	return location;
}

void GL_useProgram(GLuint program) {
	static GLuint lastUsedProgram = 0;
	glGetErrorPrettyPrint(__LINE__);
	if (program != lastUsedProgram) {
		glUseProgram(program);
		lastUsedProgram = program;
	}
	glGetErrorPrettyPrint(__LINE__);
}


// ES 1.0
//
//void GL_setTexturing(bool state) {
//	static bool lastTexturing = false;
//	if (state == lastTexturing) {
//		return;
//	}
//	lastTexturing = state;
//	state ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
//	glGetErrorPrettyPrint(__LINE__);
//}
//
//void GL_setVertexColors(bool state) {
//#ifndef USE_GL2_0
//	static int lastVertexColors = false;
//	if (state == lastVertexColors) {
//		return;
//	}
//	lastVertexColors = state;
//	if (state) {
//		glEnableClientState(GL_COLOR_ARRAY);
//	} else {
//		glDisableClientState(GL_COLOR_ARRAY);
//	}
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_setTextureCoords(bool state) {
//#ifndef USE_GL2_0
//	static bool lastTextureCoords = false;
//	if (state == lastTextureCoords) {
//		return;
//	}
//	lastTextureCoords = state;
//	state ? glEnableClientState(GL_TEXTURE_COORD_ARRAY) : glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
//#ifndef USE_GL2_0
//	static unsigned char lastR = 255;
//	static unsigned char lastG = 255;
//	static unsigned char lastB = 255;
//	static unsigned char lastA = 255;
//	
//	if (lastR == r && lastG == g && lastB == b && lastA == a) {
//		return;
//	}
//	glColor4ub(r, g, b, a);
//	lastR = r;
//	lastG = g;
//	lastB = b;
//	lastA = a;
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//
//void GL_vertexPointer(int size, int type, int stride, const void *ptr) {
//#ifndef USE_GL2_0
//	glVertexPointer(size, type, stride, ptr);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_colorPointer(int size, int type, int stride, const void *ptr) {
//#ifndef USE_GL2_0
//	glColorPointer(size, type, stride, ptr);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_texturePointer(int size, int type, int stride, const void *ptr) {
//#ifndef USE_GL2_0
//	glTexCoordPointer(size, type, stride, ptr);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_normalPointer(int size, int type, int stride, const void *ptr) {
//#ifndef USE_GL2_0
//	glNormalPointer(type, stride, ptr);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_pushMatrix() {
//#ifndef USE_GL2_0
//	glPushMatrix();
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_popMatrix() {
//#ifndef USE_GL2_0
//	glPopMatrix();
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_translate(float x, float y, float z) {
//#ifndef USE_GL2_0
//	if (x == 0 && y == 0 && z == 0) {
//		return;
//	}
//	glTranslatef(x, y, z);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_rotate(float a, float x, float y, float z) {
//#ifndef USE_GL2_0
//	if (a == 0) {
//		return;
//	}
//	glRotatef(a, x, y, z);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_scale(float x, float y, float z) {
//#ifndef USE_GL2_0
//	if (x == 1 && y == 1 && z == 1) {
//		return;
//	}
//	glScalef(x, y, z);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_setMatrices(float *projection, float *modelView) {
//#ifndef USE_GL2_0
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf(projection);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf(modelView);
//#endif
//}
//
//void GL_multMatrix(const float *data) {
//#ifndef USE_GL2_0
//	if (!data) {
//		return;
//	}
//	glMultMatrixf(data);
//	glGetErrorPrettyPrint(__LINE__);
//#endif
//}
//
//void GL_drawTriangles(int count) {
//	glDrawArrays(GL_TRIANGLES, 0, count);
//	glGetErrorPrettyPrint(__LINE__);
//}
//
//void GL_drawTriangleStrip(int count) {
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
//	glGetErrorPrettyPrint(__LINE__);
//}
//
//void GL_drawElements(int mode, int count, int type, const void *indices) {
//	glDrawElements(mode, count, type, indices);
//	glGetErrorPrettyPrint(__LINE__);
//}



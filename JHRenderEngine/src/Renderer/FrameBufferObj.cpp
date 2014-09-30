//
//  FrameBufferObj.cpp
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <assert.h>
#include <stdio.h>

#include "FrameBufferObj.h"
#include "platformGL.h"


// Check if a FBO initialization is successful.
static void checkFBOStatus() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE: 
            printf("FBO success: complete\n");
            return;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            printf("FBO failure: format unsupported\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            printf("FBO failure: incomplete attachment\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            printf("FBO failure: incomplete. missing attachment\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            printf("FBO failure: incomplete dimensions\n");
            break;
        default:
            printf("FBO failure: reason unknown\n");
            break;
    }
    assert(false);
}


// Initialize an offscreen FBO. Render to texture.
FrameBufferObj::FrameBufferObj(int w, int h) {
    mWidth = w;
    mHeight = h;

    // When sending data from client to GL, do 1-byte alignment instead of 4.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Generate the texture for color attachment.
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_HALF_FLOAT_OES, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
    
    // Generate the render buffer for depth attachment.
    glGenRenderbuffers(1, &depthId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);

    // Generate the FBO and attach the color and depth.
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthId);
    
    // Check if the initialization is correct
    checkFBOStatus();

    // Make sure the FBO is clean.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Turn it off for now. This re-enable the rendering to window.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


FrameBufferObj::~FrameBufferObj() {
    if(fboId <= 0) {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fboId);    
    // Detach the depth render buffer and color texture from the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    // Delete the texture
    glDeleteTextures(1, &texId);
    texId = 0;
    
    // Deactivate the frame buffer object and delete it.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fboId);
    fboId = 0;
}

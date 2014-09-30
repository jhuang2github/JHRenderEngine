//
//  GLES2Renderer.cpp
//
//  Created by Jingshu Huang on 1/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <iostream>

#include "Camera.h"
#include "GLES2Renderer.h"
#include "GLUtil.h"
//#include "matrix.h"   // For debug functions
#include "MaterialManager.h"
#include "Mesh.h"
#include "Pass.h"
#include "Renderable.h"
#include "SceneMananger.h"
#include "Shader.h"
#include "SubMesh.h"
#include "VertexBufferObj.h"
#include "Viewport.h"


GLES2Renderer::GLES2Renderer()
    : Renderer(SceneManager::getInstance(), MaterialManager::getInstance()) {
    getSceneManager()->setRenderer(this);
    mCurPass = NULL;
}

GLES2Renderer::~GLES2Renderer() {
}

void GLES2Renderer::render(const float secondsPassed) {
    RenderWindow* window = getRenderTarget();
    assert(window != NULL);
    window->update(secondsPassed);  // update cameras associated with the render target.
    window->render(secondsPassed);
}

void GLES2Renderer::render(Viewport* vp, Camera* cam) {
    setViewport(vp);
    setCamera(cam);

    beginFrame();
    processRenderQueues();
    endFrame();
}


void GLES2Renderer::setViewport(Viewport* vp) {
    // Note a Rect's origin is at top left corner while glViewport expects the origin
    // at the lower left corner.
    const JHEngine::Rect& rect = vp->getViewportRect();
    float targetHeight = vp->getRenderTarget()->getHeight();
    GL_viewport(rect.x, targetHeight - (rect.y + rect.height), rect.width, rect.height);
}


void GLES2Renderer::beginFrame() {
}


void GLES2Renderer::endFrame() {
    if (mCurPass) {
        mCurPass->getShader()->disableAttributes();
    }
    GL_bindTexture(GL_TEXTURE_2D, 0);
    GL_bindBuffer(GL_ARRAY_BUFFER, 0);
    GL_bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void GLES2Renderer::processRenderQueues() {
    for (RenderQueueMap::const_iterator iter = mRenderQueues.begin();
         iter != mRenderQueues.end(); ++iter) {
        assert(iter->second != NULL);
        iter->second->accept(*this);
    }
}


// Set up |pass| specific settings before visiting a renerable queue.
void GLES2Renderer::preVisit(Pass *pass) {
    // Pass environment set up.
    mCurPass = pass;
    assert(mCurPass != NULL);

    pass->useRenderState();
    pass->useShader();

    // Set up scene layer scoped params.
    Shader* curShader = pass->getShader();
    assert(curShader != NULL);
    assert(mCurCamera != NULL);

    // Set active uniforms. Note matrices in Camera/Frustum store data in
    // row-major while OpenGL is column-major. So we transpose them before
    // sending the data down to GPU.
    const ShaderUnfmList& unfms = curShader->getUniforms(UNFM_SCENE_LAYER_SCOPE);
    for (ShaderUnfmList::const_iterator iter = unfms.begin();
         iter != unfms.end(); ++iter) {
        assert(*iter);
        // TODO(jhuang): cache the transpose matrix.
        switch((*iter)->getNameId()) {
            case ShaderParam::kViewProjMatId:
                curShader->setUniformValue(UNFM_SCENE_LAYER_SCOPE,
                                           ShaderParam::kViewProjMatId,
                                           ((mCurCamera->getViewProjectionMatrixPtr())->transpose()).data);
                //std::cout << "viewProj = " << mCurCamera->getViewProjectionMatrix() << std::endl;
                break;
            case ShaderParam::kProjectionMatId:
                curShader->setUniformValue(UNFM_SCENE_LAYER_SCOPE,
                                           ShaderParam::kProjectionMatId,
                                           ((mCurCamera->getProjectionMatrixPtr())->transpose()).data);
                //std::cout << "proj = " << mCurCamera->getProjectionMatrix() << std::endl;
                break;
            case ShaderParam::kViewMatId:
                curShader->setUniformValue(UNFM_SCENE_LAYER_SCOPE,
                                           ShaderParam::kViewMatId,
                                           ((mCurCamera->getViewMatrixPtr())->transpose()).data);
                //std::cout << "view = " << mCurCamera->getViewMatrix() << std::endl;
                break;
            default:
                break;
        }
    }

    curShader->enableUniforms(UNFM_SCENE_LAYER_SCOPE);
}


// Set up |renderable| specific settings and render.
void GLES2Renderer::visit(Renderable* r) {
    assert(r != NULL && "[GLES2Renderer visit]: renderable is empty.\n");
    assert(mCurPass != NULL && "[GLES2Render visit]: current pass is empty.\n");
    Shader* curShader = mCurPass->getShader();
    assert(curShader != NULL);

    RenderState* localRenderState = r->getRenderState();
    if (localRenderState) {
        mCurPass->useRenderState(*localRenderState);
    }

    mCurPass->activateTextureUnits(r->getTextureMap());

    // Note matrices in Renderable store data in row-major while OpenGL is
    // column-major. So we transpose them before sending the data down to GPU.
    const ShaderUnfmList& unfms = curShader->getUniforms(UNFM_MODEL_SCOPE);
    for (ShaderUnfmList::const_iterator iter = unfms.begin();
         iter != unfms.end(); ++iter) {
        assert(*iter);
        switch((*iter)->getNameId()) {
            case ShaderParam::kModelMatId:
                curShader->setUniformValue(UNFM_MODEL_SCOPE,
                                           ShaderParam::kModelMatId,
                                           (r->getWorldTransform().transpose()).data);
                //std::cout << "model = " << r->getWorldTransform() << std::endl;
                break;
            default:
                break;
        }
    }
    curShader->enableUniforms(UNFM_MODEL_SCOPE);

    // Set active attributes.
    const EVertexFormat vtFormat = r->getMesh()->getVertexFormat();
    int stride = Mesh::getStride(vtFormat);
    const ShaderAttrList& attrs = curShader->getAttributes();
    for (ShaderAttrList::const_iterator iter = attrs.begin();
         iter != attrs.end(); ++iter) {
        assert(*iter);
        switch((*iter)->getNameId()) {
            case ShaderParam::kPositionId:
                curShader->setAttrStrideAndOffset(ShaderParam::kPositionId,
                                                  stride,
                                                  Mesh::getOffset(vtFormat, Mesh::kPosIndex));
                break;
            case ShaderParam::kTexCoordsId:
                curShader->setAttrStrideAndOffset(ShaderParam::kTexCoordsId,
                                                  stride,
                                                  Mesh::getOffset(vtFormat, Mesh::kTexCoordIndex));
                break;
            default:
                break;
        }
    }

    VertexVBO* vertexVBO = r->getVertexVbo();
    assert(vertexVBO != NULL);
    curShader->enableAttributes(vertexVBO->getVboID());
    IndexVBO* indexVBO = r->getIndexVbo();
    if (!indexVBO) {
        GL_drawArrays(r->getRenderMode(), r->getNumVertices(), 0);
        
    } else {
        GL_bindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO->getVboID());
        GL_drawElements(r->getRenderMode(), r->getNumIndices(), r->getIndexFormat(), 0);
    }

    // Reset the render state to the Pass' specific one so other renderables
    // in the render queue do not get messed up.
    if (localRenderState) {
        mCurPass->useRenderState();
    }
}


// Do some post-visit job after visiting a renerable queue.
void GLES2Renderer::postVisit(Pass* pass) {
    assert(mCurPass != NULL);
}


//// Debug use only. identity projection and model view matrix.
//void GLES2Renderer::debugTransformations() {
//	GLfloat proj[16];
//    GLfloat view[16];
//    GLfloat model[16];
//    GLfloat viewProj[16];
//	mat4f_LoadOrtho(-1.f, 1.f, -1.f, 1.f, -1.0f, 1.0f, proj);
//    //    mat4f_LoadPerspective(1.f * kRadianPerDegree, 1.f, 0.01f, 1000.f, proj);
//    mat4f_LoadIdentity(view);
//    mat4f_LoadIdentity(model);
//	mat4f_MultiplyMat4f(proj, view, viewProj);
//    
//    Shader* curShader = mCurPass->getShader();
//    
//    // scene scope
//    curShader->setUniformValue(UNFM_SCENE_LAYER_SCOPE,
//                               ShaderParam::kViewProjMatId,
//                               viewProj);
////    curShader->setUniformValue(UNFM_SCENE_LAYER_SCOPE,
////                               ShaderParam::kProjectionMatId,
////                               proj);
////    curShader->setUniformValue(UNFM_SCENE_LAYER_SCOPE,
////                               ShaderParam::kViewMatId,
////                               view);
//    curShader->enableUniforms(UNFM_SCENE_LAYER_SCOPE);
//    
//    // model scope
//    curShader->setUniformValue(UNFM_MODEL_SCOPE,
//                               ShaderParam::kModelMatId,
//                               model);
//    curShader->enableUniforms(UNFM_MODEL_SCOPE);
//}
//
//
//void GLES2Renderer::debugDraw() {
//    static const GLfloat squareVertices[] = {
//        -0.5f, -0.5f, 0,
//        0.5f,  -0.5f, 0,
//        -0.5f,  0.5f, 0,
//        0.5f,   0.5f, 0,
//    };
//	GL_vertexAttribPointer(0, 3, GL_FLOAT, 0, 0, squareVertices);
//	GL_enableVertexAttribArray(0);
//    GL_drawArrays(GL_TRIANGLE_STRIP, 0, 4);
//}

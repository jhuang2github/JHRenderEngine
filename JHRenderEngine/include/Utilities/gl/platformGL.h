//
//  platform.h
//
//  Created by Jingshu Huang on 2/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _platform_h
#define _platform_h

#ifdef __APPLE__

// iOS specific
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#else

// Android specific
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

#endif

#endif

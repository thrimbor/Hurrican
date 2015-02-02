#ifndef _OPENGL_H_
#define _OPENGL_H_

#include "SDL.h"

// Do some sanity checks
#if !defined(USE_GL1) && !defined(USE_GL2)
#error "ERROR USE_GL1 nor USE_GL2 are defined. Only one must be defined."
#elif defined(USE_GL1) && defined(USE_GL2)
#error "ERROR USE_GL1 and USE_GL2 are defined. Only one must be defined."
#elif defined(USE_GL1) && defined(USE_GLES2)
#error "ERROR USE_GL1 and USE_GLES2 are defined. Replace USE_GLES2 for USE_GLES1"
#elif defined(USE_GL2) && defined(USE_GLES1)
#error "ERROR USE_GL2 and USE_GLES1 are defined. Replace USE_GLES1 for USE_GLES2"
#endif

#if !defined(__WIN32)
#define GL_GLEXT_PROTOTYPES 1
#endif

#if SDL_VERSION_ATLEAST(2,0,0)

#if !defined(USE_GLES1) && !defined(USE_GLES2) && defined(USE_GLFULL)
#include "SDL_opengl.h"
#elif !defined(USE_GLES1) && defined(USE_GLES2) && !defined(USE_GLFULL)
#include "SDL_opengles2.h"
#elif defined(USE_GLES1) && !defined(USE_GLES2) && !defined(USE_GLFULL)
#include "SDL_opengles.h"
#else
#error Invalid OpenGL Config
#endif

#else /* SDL 1.2 */

#if defined(USE_GLES1) || defined(USE_GLES2)
/* OpenGL-ES Profiles */
#if defined(USE_GLES2)
#include "gl2.h"
#include "gl2ext.h"
#else
#include "gl.h"
#include "glext.h"
#endif

#else /* Full OpenGL Profile */
#include "SDL_opengl.h"
#endif

#endif

#if defined(USE_GLES1) || defined(USE_GLES2)
#define glClearDepth glClearDepthf
#endif

#if defined(__WIN32__)
#if defined(USE_GL2)
/* OpenGL Version 2.0 API */
extern PFNGLDELETESHADERPROC                glDeleteShader;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
extern PFNGLGETACTIVEATTRIBPROC             glGetActiveAttrib;
extern PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation;
extern PFNGLGETACTIVEUNIFORMPROC            glGetActiveUniform;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM4FVPROC                  glUniform4fv;
extern PFNGLUNIFORMMATRIX3FVPROC            glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
/* FBO API */
#if defined(USE_FBO)
#define GL_FRAMEBUFFER                                  GL_FRAMEBUFFER_EXT
#define GL_FRAMEBUFFER_COMPLETE                         GL_FRAMEBUFFER_COMPLETE_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT            GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS            GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_UNSUPPORTED                      GL_FRAMEBUFFER_UNSUPPORTED_EXT
#define GL_COLOR_ATTACHMENT0                            GL_COLOR_ATTACHMENT0_EXT
#define glBindFramebuffer                   glBindFramebufferEXT
#define glDeleteFramebuffers                glDeleteFramebuffersEXT
#define glGenFramebuffers                   glGenFramebuffersEXT
#define glCheckFramebufferStatus            glCheckFramebufferStatusEXT
#define glFramebufferTexture2D              glFramebufferTexture2DEXT
#define glGenRenderbuffers                  glGenRenderbuffersEXT
#define glBindRenderbuffer                  glBindRenderbufferEXT
#define glRenderbufferStorage               glRenderbufferStorageEXT
#define glDeleteRenderbuffers               glDeleteRenderbuffersEXT
extern PFNGLBINDFRAMEBUFFEREXTPROC          glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC       glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSEXTPROC          glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC   glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC     glFramebufferTexture2D;
extern PFNGLGENRENDERBUFFERSEXTPROC         glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFEREXTPROC         glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC      glRenderbufferStorage;
extern PFNGLDELETERENDERBUFFERSEXTPROC      glDeleteRenderbuffers;
#endif /* defined(USE_FBO) */
#endif

#if defined(USE_GL1) || defined(USE_GL2)
/* GL_ARB_texture_compression */
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC     glCompressedTexImage2D;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC    glGetCompressedTexImageARB;
#endif
#endif /* WIN32 */

#endif /* _OPENGL_H_ */

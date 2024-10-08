#pragma once

#ifdef PLATFORM_ANDROID

#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>

#else

#include <GL/glew.h>

#include <GL/gl.h>

#endif

#include <string>


void checkGlError();

void checkGlError( std::string const& info );

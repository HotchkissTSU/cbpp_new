#include "cbpp/Graphics.h"
/*
#include "cbpp/Filesystem.h"
#include "cbpp/Error.h"
#include "cbpp/Memory.h"

#include <stdio.h>

namespace cbpp {
    const char* GetShaderClassExtension(GLenum iClass) {
        switch( iClass ) {
            case GL_VERTEX_SHADER:      return "vtx";
            case GL_FRAGMENT_SHADER:    return "frag";
            case GL_GEOMETRY_SHADER:    return "geom";
            default:                    return "txt";
        }
    }

    GLuint CShaderPipe::CompileShader(const char* sName, GLenum iShaderClass, char* sBuffer) {
        snprintf(sBuffer, 100, "shaders/%s.%s", sName, GetShaderClassExtension(iShaderClass));

        IFile* pFile = OpenAsset(sBuffer, "rb");

        if(pFile == NULL) {
            WriteLogf(ELogLevel::Error, "Unable to open said shader source file");
            return 0;
        }

        const size_t iLength = pFile->Length();
        sBuffer = Realloc(sBuffer, iLength+1);
        sBuffer[iLength] = '\0';

        pFile->ReadAll(sBuffer);

        GLuint iShader = glCreateShader(iShaderClass);
        glShaderSource(iShader, 1, &sBuffer, NULL);
        glCompileShader(iShader);

        GLint iStatus = GL_FALSE;
        glGetShaderiv(iShader, GL_COMPILE_STATUS, &iStatus);
        
        if (iStatus != GL_TRUE) {
            GLint iLogLength = 0;
            glGetShaderiv(iShader, GL_INFO_LOG_LENGTH, &iLogLength);

            sBuffer = Realloc(sBuffer, iLogLength+1);
            sBuffer[iLogLength] = '\0';

            glGetShaderInfoLog(iShader, iLogLength, NULL, sBuffer);
            glDeleteShader(iShader);

            WriteLogf(ELogLevel::Error, "Compilation of shader '%s.%s' has failed: %s", sName, GetShaderClassExtension(iShaderClass), sBuffer);

            return 0;
        }

        return iShader;
    }

    bool CShaderPipe::CompilePipeline(const char* sVtx, const char* sFrag, const char* sGeom) {
        char* sBuffer = Malloc<char>(100);

        GLuint iVertex = this->CompileShader(sVtx, GL_VERTEX_SHADER, sBuffer);
        GLuint iFragment = this->CompileShader(sFrag, GL_FRAGMENT_SHADER, sBuffer);

        GLuint iGeometry;
        if(sGeom != NULL) {
            iGeometry = this->CompileShader(sGeom, GL_GEOMETRY_SHADER, sBuffer);
        }

        if(iVertex == 0 || iGeometry == 0 || (sGeom != NULL && iGeometry == 0)) {
            WriteLogf(ELogLevel::Error, "Shaders compilation failed, check above log for details");
            Free(sBuffer);
            return false;
        }

        GLuint iPipe = glCreateProgram();
        glAttachShader(iPipe, iVertex);
        glAttachShader(iPipe, iFragment);

        if(sGeom != NULL) {
            glAttachShader(iPipe, iGeometry);
        }

        glLinkProgram(iPipe);

        GLint iStatus = GL_FALSE;
        glGetProgramiv(iPipe, GL_LINK_STATUS, &iStatus);

        if(iStatus != GL_TRUE) {
            GLint iLogLen = 0;
            glGetProgramiv(iPipe, GL_INFO_LOG_LENGTH, &iLogLen);

            sBuffer = Realloc<char>(sBuffer, iLogLen+1);
            sBuffer[iLogLen] = '\0';

            glGetProgramInfoLog(iPipe, iLogLen, NULL, sBuffer);
            
            WriteLogf(ELogLevel::Error, "Shader pipe linking has failed: %s", sBuffer);

            glDeleteProgram(iPipe);
            Free(sBuffer);
            return false;
        }
        
        if (iVertex) { glDetachShader(iPipe, iVertex); glDeleteShader(iVertex); }
        if (iFragment) { glDetachShader(iPipe, iVertex); glDeleteShader(iVertex); }
        if (iGeometry) { glDetachShader(iPipe, iGeometry); glDeleteShader(iGeometry); }

        Free(sBuffer);

        return true;
    }
}
*/
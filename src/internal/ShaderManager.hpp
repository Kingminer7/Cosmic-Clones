#pragma once
#include "../ui/CloneStyleSetting.hpp"

// inheriting ccobject for scheduler update rip
class ShaderManager : public cocos2d::CCObject {
protected:
    GLuint m_glCosmic = 0;
    GLuint m_glNormal = 0;
    GLuint m_glOverlay = 0;
    GLint m_glTime = 0;
    GLint m_glScreenSize = 0;

    float m_time = 0;
    bool m_hasSetup = false;

    cocos2d::CCGLProgram* m_shader = nullptr;
public:
    static ShaderManager& get();
    cocos2d::CCGLProgram* getCosmicShader() const;
    void setup();
    void update(float dt);
};

#pragma once
#include "CloneStyleSetting.hpp"

class ShaderManager {
public:
    static ShaderManager& get();
    cocos2d::CCGLProgram* getCosmicShader();
};

class CosmicSprite : public cocos2d::CCSprite {
protected:
    bool init() override;
    void draw() override;
    void visit() override;
    void update(float dt) override;

    GLuint m_cosmicTex = 0;;
    GLuint m_normalTex = 0;
    GLuint m_overlayTex = 0;
    Style m_style;
    float m_time = 0;
    cocos2d::CCRenderTexture* m_renderTexture = nullptr;
public:
    static CosmicSprite* create();

    void updateStyle(Style style);
    void resetRenderTexture();
    cocos2d::CCRenderTexture* getRenderTexture();
};

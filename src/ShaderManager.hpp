#pragma once
#include "CloneStyleSetting.hpp"

#include <alphalaneous.alphas-ui-pack/include/nodes/RenderNode.hpp>

class ShaderManager {
public:
    static ShaderManager& get();
    cocos2d::CCGLProgram* getCosmicShader();
};

class CosmicSprite : public alpha::ui::RenderNode {
protected:
    bool init(CCNode* node, bool constrain);
    void draw() override;
    void update(float dt) override;

    GLuint m_cosmicTex = 0;;
    GLuint m_normalTex = 0;
    GLuint m_overlayTex = 0;
    Style m_style;
    float m_time = 0;
public:
    static CosmicSprite* create(CCNode* node, bool constrain = false);

    void updateStyle(Style style);
};

#pragma once

class ShaderManager {
public:
    static ShaderManager& get();
    cocos2d::CCGLProgram* getCosmicShader();
};

class CosmicSprite : public cocos2d::CCSprite {
protected:
    bool init(cocos2d::CCTexture2D* base);
    void draw();
    void update(float dt);

    GLuint m_cosmicTex = 0;;
    GLuint m_normalTex = 0;
    GLuint m_overlayTex = 0;

    std::string m_style;

    float m_time = 0;

public:
    static CosmicSprite* create(cocos2d::CCTexture2D* base);

    void updateStyle(std::string style);
};

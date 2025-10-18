#pragma once

#include <Geode/modify/PlayerObject.hpp>

enum class Animation {
    Jump,
    Fall,
    Idle,
    Run,
    None
};

struct Snapshot {
    cocos2d::CCPoint pos;
    float rotation;
    float scale;
    bool left = false;
    bool side = false;
    bool visible;
    IconType type;
    bool flip;
    Animation anim = Animation::None;

    // p2
    bool dualEnabled = false;
    cocos2d::CCPoint pos2;
    float rotation2;
    float scale2;
    bool left2 = false;
    bool side2 = false;
    bool visible2;
    IconType type2;
    bool flip2;
    Animation anim2 = Animation::None;
};

class CosmicClone;

class $modify(CosmicPlayerObject, PlayerObject) {
    struct Fields {
        bool m_isCosmic = false;
        CosmicClone* m_clone = nullptr;
    };

    static CosmicPlayerObject* createCosmic(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer);
};

enum class CosmicCloneSFXType {
    FirstSpawn,
    Spawn,
    Die,
};

class CosmicClone {
protected:
    int m_delay = 0;
    CosmicPlayerObject* m_p1 = nullptr;
    CosmicPlayerObject* m_p2 = nullptr;
    std::string m_style;

    bool m_dual = false;
    IconType m_p1type = IconType::Cube;
    IconType m_p2type = IconType::Cube;
    Animation m_p1anim = Animation::None;
    Animation m_p2anim = Animation::None;

    void init(int delay);
public:
    static std::shared_ptr<CosmicClone> create(int delay);
    CosmicPlayerObject* getP1() const;
    CosmicPlayerObject* getP2() const;
    int getDelay() const;
    void checkCollision(PlayerObject* player) const;
    void setDual(bool dual);
    IconType getType(int player) const;
    void setType(IconType type, int player);
    Animation getAnimation(int player) const;
    void animate(Animation animation, int player);
    void updateAnimation(int player) const;

    int playSFX(CosmicCloneSFXType type);

    void updateStyle(std::string style);
};

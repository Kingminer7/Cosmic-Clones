#include "CosmicClone.hpp"

#include <utility>
#include "Utils.hpp"

using namespace geode::prelude;

int getFrame(const IconType type) {
    const auto gm = GameManager::sharedState();
    switch (type) {
        default: return gm->getPlayerFrame();
        case IconType::Ship: return gm->getPlayerShip();
        case IconType::Ball: return gm->getPlayerBall();
        case IconType::Ufo: return gm->getPlayerBird();
        case IconType::Wave: return gm->getPlayerDart();
        case IconType::Robot: return gm->getPlayerRobot();
        case IconType::Spider: return gm->getPlayerSpider();
        case IconType::Swing: return gm->getPlayerSwing();
        case IconType::Jetpack: return gm->getPlayerJetpack();
    }
}

void CosmicClone::init(const int delay) {
    m_delay = delay;
    const auto pl = PlayLayer::get();
    PlayerObject *plr = pl->m_player1;

    auto gm = GameManager::sharedState();
    m_p1 = CosmicPlayerObject::createCosmic(gm->getPlayerFrame(), 1, pl, pl, true);
    m_p1->m_fields->m_clone = this;
    m_p1->setID(fmt::format("cosmic-clone-{}"_spr, delay));
    m_p1->togglePlatformerMode(true);

    auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
    m_p2 = CosmicPlayerObject::createCosmic(sdi ? sdi->getSavedValue<int>("cube", gm->getPlayerFrame()) : gm->getPlayerFrame(), 1, pl, pl, true);
    m_p2->m_fields->m_clone = this;
    m_p2->setID(fmt::format("cosmic-clone-dual-{}"_spr, delay));
    m_p2->togglePlatformerMode(true);

    updateStyle(getSettingFast<"style", std::string>());
}

std::shared_ptr<CosmicClone> CosmicClone::create(const int delay) {
    auto ret = std::make_shared<CosmicClone>();
    ret->init(delay);
    return ret;
}

CosmicPlayerObject *CosmicClone::getP1() const {
    return m_p1;
}

CosmicPlayerObject *CosmicClone::getP2() const {
    return m_p2;
}

int CosmicClone::getDelay() const {
    return m_delay;
}

void CosmicClone::checkCollision(PlayerObject* player) const {
    const auto pl = PlayLayer::get();
    if (m_p1->getObjectRect().intersectsRect(player->getObjectRect()) || (m_dual && m_p2->getObjectRect().intersectsRect(player->getObjectRect()))) {
        pl->destroyPlayer(player, nullptr);
    }
}

void CosmicClone::setDual(const bool dual) {
    m_p2->setVisible(dual);
    m_dual = dual;
}

void CosmicClone::setType(const IconType type, const int player) {
    auto gm = GameManager::get();
    if (player == 1) {
        if (type == m_p1type) return;
        m_p1type = type;
        switch (type) {
            case IconType::Cube:
                m_p1->toggleFlyMode(false, false);
                m_p1->toggleRollMode(false, false);
                m_p1->toggleBirdMode(false, false);
                m_p1->toggleDartMode(false, false);
                m_p1->toggleRobotMode(false, false);
                m_p1->toggleSpiderMode(false, false);
                m_p1->toggleSwingMode(false, false);
                m_p1->updatePlayerFrame(gm->getPlayerFrame());
                break;
            case IconType::Ship:
                m_p1->toggleFlyMode(true, true);
                if (m_p1->m_isPlatformer) {
                    m_p1->updatePlayerJetpackFrame(gm->getPlayerJetpack());
                } else {
                    m_p1->updatePlayerShipFrame(gm->getPlayerShip());
                }
                break;
            case IconType::Ball:
                m_p1->toggleRollMode(true, true);
                m_p1->updatePlayerRollFrame(gm->getPlayerBall());
                break;
            case IconType::Ufo:
                m_p1->toggleBirdMode(true, true);
                m_p1->updatePlayerBirdFrame(gm->getPlayerBird());
                break;
            case IconType::Wave:
                m_p1->toggleDartMode(true, true);
                m_p1->updatePlayerDartFrame(gm->getPlayerDart());
                break;
            case IconType::Robot:
                m_p1->toggleRobotMode(true, true);
                m_p1->updatePlayerRobotFrame(gm->getPlayerRobot());
                break;
            case IconType::Spider:
                m_p1->toggleSpiderMode(true, true);
                m_p1->updatePlayerSpiderFrame(gm->getPlayerSpider());
                break;
            case IconType::Swing:
                m_p1->toggleSwingMode(true, true);
                m_p1->updatePlayerSwingFrame(gm->getPlayerSwing());
                break;
            default: break;
        }
    } else if (player == 2) {
        if (type == m_p2type) return;
        m_p2type = type;
        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        switch (type) {
            case IconType::Cube:
                m_p2->toggleFlyMode(false, false);
                m_p2->toggleRollMode(false, false);
                m_p2->toggleBirdMode(false, false);
                m_p2->toggleDartMode(false, false);
                m_p2->toggleRobotMode(false, false);
                m_p2->toggleSpiderMode(false, false);
                m_p2->toggleSwingMode(false, false);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("cube", gm->getPlayerFrame()) : gm->getPlayerFrame());
                break;
            case IconType::Ship:
                m_p2->toggleFlyMode(true, true);
                if (m_p2->m_isPlatformer){
                    m_p2->updatePlayerJetpackFrame(sdi ? sdi->getSavedValue<int>("jetpack", gm->getPlayerJetpack()) : gm->getPlayerJetpack());
                } else {
                    m_p2->updatePlayerShipFrame(sdi ? sdi->getSavedValue<int>("ship", gm->getPlayerShip()) : gm->getPlayerShip());
                }
                break;
            case IconType::Ball:
                m_p2->toggleRollMode(true, true);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("roll", gm->getPlayerBall()) : gm->getPlayerBall());
                break;
            case IconType::Ufo:
                m_p2->toggleBirdMode(true, true);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("bird", gm->getPlayerBird()) : gm->getPlayerBird());
                break;
            case IconType::Wave:
                m_p2->toggleDartMode(true, true);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("dart", gm->getPlayerDart()) : gm->getPlayerDart());
                break;
            case IconType::Robot:
                m_p2->toggleRobotMode(true, true);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("robot", gm->getPlayerRobot()) : gm->getPlayerRobot());
                break;
            case IconType::Spider:
                m_p2->toggleSpiderMode(true, true);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("spider", gm->getPlayerSpider()) : gm->getPlayerSpider());
                break;
            case IconType::Swing:
                m_p2->toggleSwingMode(true, true);
                m_p2->updatePlayerFrame(sdi ? sdi->getSavedValue<int>("swing", gm->getPlayerSwing()) : gm->getPlayerSwing());
                break;
            default: break;
        }
    }
    updateAnimation(player);
}

auto CosmicClone::getAnimation(const int player) const -> ::Animation {
    if (player == 1) return m_p1anim;
    return m_p2anim;
}

void CosmicClone::animate(const ::Animation animation, const int player) {
    if (player == 1) {
        if (m_p1anim == animation) return;
        m_p1anim = animation;
        updateAnimation(player);
    } else if (player == 2) {
        if (m_p2anim == animation) return;
        m_p2anim = animation;
        updateAnimation(player);
    }
}

void CosmicClone::updateAnimation(const int player) const {
    if (player == 1) {
        switch (m_p1anim) {
            case ::Animation::None: break;
            case ::Animation::Jump: {
                if (m_p1type == IconType::Cube) {
                    m_p1->animatePlatformerJump(1.f);
                } else if (m_p1type == IconType::Robot) {
                    m_p1->m_robotSprite->tweenToAnimation("jump_loop", 0.1f);
                } else if (m_p1type == IconType::Spider) {
                    m_p1->m_spiderSprite->tweenToAnimation("jump_loop", 0.1f);
                }
                break;
            }
            case ::Animation::Run: {
                if (m_p1type == IconType::Robot) {
                    m_p1->m_robotSprite->tweenToAnimation("run", 0.1f);
                } else if (m_p1type == IconType::Spider) {
                    m_p1->m_spiderSprite->tweenToAnimation("run", 0.1f);
                }
                break;
            }
            case ::Animation::Idle: {
                if (m_p1type == IconType::Robot) {
                    m_p1->m_robotSprite->tweenToAnimation("idle01", 0.1f);
                } else if (m_p1type == IconType::Spider) {
                    m_p1->m_spiderSprite->tweenToAnimation("idle01", 0.1f);
                }
                break;
            }
            case ::Animation::Fall: {
                if (m_p1type == IconType::Robot) {
                    m_p1->m_robotSprite->tweenToAnimation("fall_loop", 0.1f);
                } else if (m_p1type == IconType::Spider) {
                    m_p1->m_spiderSprite->tweenToAnimation("fall_loop", 0.1f);
                }
                break;
            }
        }
    } else if (player == 2) {
        switch (m_p2anim) {
            case ::Animation::None: break;
            case ::Animation::Jump: {
                if (m_p2type == IconType::Cube) {
                    m_p2->animatePlatformerJump(1.f);
                } else if (m_p2type == IconType::Robot) {
                    m_p2->m_robotSprite->tweenToAnimation("jump_loop", 0.1f);
                } else if (m_p2type == IconType::Spider) {
                    m_p2->m_spiderSprite->tweenToAnimation("jump_loop", 0.1f);
                }
                break;
            }
            case ::Animation::Run: {
                if (m_p2type == IconType::Robot) {
                    m_p2->m_robotSprite->tweenToAnimation("run", 0.1f);
                } else if (m_p2type == IconType::Spider) {
                    m_p2->m_spiderSprite->tweenToAnimation("run", 0.1f);
                }
            }
            case ::Animation::Idle: {
                if (m_p2type == IconType::Robot) {
                    m_p2->m_robotSprite->tweenToAnimation("idle01", 0.1f);
                } else if (m_p2type == IconType::Spider) {
                    m_p2->m_spiderSprite->tweenToAnimation("idle01", 0.1f);
                }
            }
            case ::Animation::Fall: {
                if (m_p2type == IconType::Robot) {
                    m_p2->m_robotSprite->tweenToAnimation("fall_loop", 0.1f);
                } else if (m_p2type == IconType::Spider) {
                    m_p2->m_spiderSprite->tweenToAnimation("fall_loop", 0.1f);
                }
            }
        }
    }
}

IconType CosmicClone::getType(const int player) const {
    return player == 2 ? m_p2type : m_p1type;
}

CosmicPlayerObject *CosmicPlayerObject::createCosmic(int player, int ship, GJBaseGameLayer *gameLayer, CCLayer *layer,
                                                     bool playLayer) {
    auto ret = reinterpret_cast<CosmicPlayerObject *>(PlayerObject::create(player, ship, gameLayer, layer, playLayer));
    ret->m_fields->m_isCosmic = true;
    return ret;
}

void CosmicClone::updateStyle(std::string style) {
    m_style = std::move(style);

    for (auto plr : {m_p1, m_p2}) {
        if (m_style == "Cosmic Mario (SMG 1)") {
            // This won't really be seen normally but just in case
            plr->setColor(ccColor3B{12, 11, 56});
            plr->setSecondColor(ccColor3B{11, 27, 56});
            plr->setGlowColor(ccColor3B{13, 23, 64});
            plr->m_hasGlow = true;
            plr->updateGlowColor();
            plr->toggleGhostEffect(GhostType::Disabled);
        } else if (m_style == "Cosmic Clone (SMG 2)") {
            plr->setColor(ccColor3B{60, 20, 21});
            plr->setSecondColor(ccColor3B{243, 235, 87});
            plr->enableCustomGlowColor(ccColor3B{193, 50, 54});
            plr->m_hasGlow = true;
            plr->updateGlowColor();
            plr->toggleGhostEffect(GhostType::Disabled);
        } else if (m_style == "Badeline Chaser (Celeste)") {
            plr->setColor(ccColor3B{155, 63, 181});
            plr->setSecondColor(ccColor3B{191, 29, 51});
            plr->disableCustomGlowColor();
            plr->m_hasGlow = false;
            plr->updateGlowColor();
            plr->toggleGhostEffect(GhostType::Enabled);
            plr->m_ghostTrail->m_color = ccColor3B{255, 0, 0};
            plr->m_ghostTrail->m_fadeInterval = .4f;
        } else {
            plr->setColor(getSettingFast<"custom-col1", ccColor3B>());
            plr->setSecondColor(getSettingFast<"custom-col2", ccColor3B>());
            if (getSettingFast<"custom-use-glow", bool>()) {
                plr->enableCustomGlowColor(getSettingFast<"custom-glow", ccColor3B>());
                plr->m_hasGlow = true;
            } else {
                plr->disableCustomGlowColor();
                plr->m_hasGlow = false;
            }
            plr->updateGlowColor();
            plr->toggleGhostEffect(GhostType::Disabled);
        }
    }
}

int CosmicClone::playSFX(CosmicCloneSFXType type) {
    switch (type) {
    case CosmicCloneSFXType::Spawn:
        if (m_style == "Badeline Chaser (Celeste)") {
            return FMODAudioEngine::get()->playEffect("appear.wav"_spr, 1, 0, .35);
        }
        return -1;
    case CosmicCloneSFXType::FirstSpawn:
        if (m_style == "Cosmic Clone (SMG 2)" || m_style == "Cosmic Mario (SMG 1)") {
            return FMODAudioEngine::get()->playEffect("spawn.wav"_spr);
        }
        if (m_style == "Badeline Chaser (Celeste)") {
            return FMODAudioEngine::get()->playEffect("appear.wav"_spr, 1, 0, .35);
        }
        return -1;
    case CosmicCloneSFXType::Die:
        if (m_style == "Cosmic Clone (SMG 2)" || m_style == "Cosmic Mario (SMG 1)") {
            return FMODAudioEngine::get()->playEffect("defeat.wav"_spr);
        }
        if (m_style == "Badeline Chaser (Celeste)") {
            return FMODAudioEngine::get()->playEffect("disappear.wav"_spr);
        }
        return -1;
    }
    return -1;
}

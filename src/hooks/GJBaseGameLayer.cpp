#include "GJBaseGameLayer.hpp"

#include "../Utils.hpp"

void CosmicClonesGJBGL::processCommands(float dt) {
    if (!PlayLayer::get()) return GJBaseGameLayer::processCommands(dt);
    auto fields = m_fields.self();
    if (!fields->m_enabled || fields->m_stopped) return GJBaseGameLayer::processCommands(dt);
    int tick = m_gameState.m_currentProgress - fields->m_offset;
    for (auto clone : fields->m_clones) {
        auto del = tick - clone->getDelay();
        if (fields->m_snapshots.contains(del)) {
            auto snap = fields->m_snapshots.at(del);
            clone->setDual(snap.dualEnabled);
            auto p1 = clone->getP1();
            p1->setPosition(snap.pos);
            p1->setScale(snap.scale);
            p1->setRotation(snap.rotation);
            p1->setVisible(snap.visible);
            p1->m_isGoingLeft = snap.left;
            p1->m_mainLayer->setScaleX(snap.left ? -1 : 1);
            p1->m_isSideways = snap.side;
            p1->flipGravity(snap.flip, true);
            clone->setType(snap.type, 1);
            clone->animate(snap.anim, 1);
            if (snap.dualEnabled) {
                auto p2 = clone->getP2();
                p2->setPosition(snap.pos2);
                p2->setScale(snap.scale2);
                p2->setRotation(snap.rotation2);
                p2->setVisible(snap.visible2);
                p2->m_isGoingLeft = snap.left2;
                p2->m_mainLayer->setScaleX(snap.left2 ? -1 : 1);
                p2->m_isSideways = snap.side2;
                p2->flipGravity(snap.flip2, true);
                clone->setType(snap.type2, 2);
                clone->animate(snap.anim2, 2);
            }
        }
    }
    for (int i = 1; i <= fields->m_count; i++) {
        auto del = ((i - 1) * fields->m_delay + fields->m_initialDelay) * 240;
        if (tick == del) {
            auto clone = CosmicClone::create(del, m_isPlatformer);
            fields->m_renderLayer->addChild(clone->getP1());
            fields->m_renderLayer->addChild(clone->getP2());
            fields->m_clones.push_back(clone);
            auto id = clone->playSFX(i > 1 ? CosmicCloneSFXType::Spawn : CosmicCloneSFXType::FirstSpawn);
            if (id > -1) fields->m_sfxIds.push_back(id);
        }
    }
    int max = tick - ((fields->m_count - 1) * fields->m_delay + fields->m_initialDelay) * 240;
    if (!m_isPracticeMode && fields->m_snapshots.contains(max)) {
        fields->m_snapshots.erase(max);
    }
    auto snap = Snapshot{
        .pos = m_player1->m_position,
        .rotation = m_player1->getRotation(),
        .scale = m_player1->getScale(),
        .left = m_player1->m_isGoingLeft,
        .side = m_player1->m_isSideways,
        .visible = m_player1->isVisible(),
        .flip = m_player1->m_isUpsideDown,
    };
    if (m_player1->m_isShip) {
        snap.type = IconType::Ship;
    } else if (m_player1->m_isBall) {
        snap.type = IconType::Ball;
    } else if (m_player1->m_isBird) {
        snap.type = IconType::Ufo;
    } else if (m_player1->m_isDart) {
        snap.type = IconType::Wave;
    } else if (m_player1->m_isRobot) {
        snap.type = IconType::Robot;
    } else if (m_player1->m_isSpider) {
        snap.type = IconType::Spider;
    } else if (m_player1->m_isSwing) {
        snap.type = IconType::Swing;
    } else {
        snap.type = IconType::Cube;
    }
    bool move = m_player1->m_platformerXVelocity > 0.1 || m_player1->m_platformerXVelocity < -0.1;
    int vel = m_player1->m_yVelocity * (m_player1->m_isUpsideDown ? -1 : 1);
    if (!m_player1->m_isOnGround && vel < -0.1) {
        snap.anim = ::Animation::Fall;
    } else if (m_player1->m_isOnGround && move) {
        snap.anim = ::Animation::Run;
    } else if (m_player1->m_isOnGround && !move) {
        snap.anim = ::Animation::Idle;
    } else if (!m_player1->m_isOnGround && vel > 0.1 && fields->m_p1Jump) {
        fields->m_p1Jump = false;
        snap.anim = ::Animation::Jump;
    }

    if (m_gameState.m_isDualMode) {
        snap.dualEnabled = true;
        snap.pos2 = m_player2->getPosition();
        snap.rotation2 = m_player2->getRotation();
        snap.scale2 = m_player2->getScale();
        snap.left2 = m_player2->m_isGoingLeft;
        snap.side2 = m_player2->m_isSideways;
        snap.flip2 = m_player2->m_isUpsideDown;
        snap.visible2 = m_player2->isVisible();
        if (m_player2->m_isShip) {
            snap.type2 = IconType::Ship;
        } else if (m_player2->m_isBall) {
            snap.type2 = IconType::Ball;
        } else if (m_player2->m_isBird) {
            snap.type2 = IconType::Ufo;
        } else if (m_player2->m_isDart) {
            snap.type2 = IconType::Wave;
        } else if (m_player2->m_isRobot) {
            snap.type2 = IconType::Robot;
        } else if (m_player2->m_isSpider) {
            snap.type2 = IconType::Spider;
        } else if (m_player2->m_isSwing) {
            snap.type2 = IconType::Swing;
        } else {
            snap.type2 = IconType::Cube;
        }
    }
    fields->m_snapshots[tick] = std::move(snap);

    GJBaseGameLayer::processCommands(dt);
    if (getSettingFast<"friendly", bool>()) return;
    if (!fields->m_p1Frozen && fields->m_p1Immunity > 0) {
        fields->m_p1Immunity--;
    } else if (!fields->m_p1Frozen) {
        for (auto const& clone : fields->m_clones) {
            if (clone->getDelay() <= tick) clone->checkCollision(m_player1);
        }
    }
    if (!fields->m_p2Frozen && fields->m_p2Immunity > 0) {
        fields->m_p2Immunity--;
    } else if (!fields->m_p2Frozen && m_gameState.m_isDualMode && m_player2 != nullptr) {
        for (auto const& clone : fields->m_clones) {
            if (clone->getDelay() <= tick) clone->checkCollision(m_player2);
        }
    }
}

void CosmicClonesGJBGL::visit() {
    if (this != static_cast<GJBaseGameLayer*>(PlayLayer::get())) return GJBaseGameLayer::visit();

    auto fields = m_fields.self();
    if (!fields->m_enabled) return GJBaseGameLayer::visit();

    float dirX = (m_objectLayer->getScaleX() > 0) ? 1.f : -1.f;
    float dirY = (m_objectLayer->getScaleY() > 0) ? 1.f : -1.f;
    
    float offsetX = (m_objectLayer->getScaleX() > 0) ? 0.f : fields->m_sprite->getContentWidth();
    float offsetY = (m_objectLayer->getScaleY() > 0) ? 0.f : fields->m_sprite->getContentHeight();

    fields->m_renderLayer->setPosition(
        m_objectLayer->getPosition() * cocos2d::CCPoint{dirX, dirY} +
        cocos2d::CCPoint{offsetX, offsetY}
    );

    fields->m_sprite->setScaleX(std::abs(1 / m_objectLayer->getScaleX()));
    fields->m_sprite->setScaleY(std::abs(1 / m_objectLayer->getScaleY()));
    fields->m_renderLayer->setScale(
        std::abs(m_objectLayer->getScaleX()),
        std::abs(m_objectLayer->getScaleY())
    );

    fields->m_renderTex->beginWithClear(0, 0, 0, 0);
    fields->m_renderLayer->visit();
    fields->m_renderTex->end();

    fields->m_sprite->setPosition(
        (fields->m_sprite->getScaledContentSize() * cocos2d::CCPoint{dirX, dirY} / 2) -
        (m_objectLayer->getPosition() / cocos2d::CCPoint{
            m_objectLayer->getScaleX(),
            m_objectLayer->getScaleY()
        })
    );

    GJBaseGameLayer::visit();
}

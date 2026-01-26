#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "../ShaderManager.hpp"
using namespace geode::prelude;

class $modify(ClonesPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto spr = CCSprite::create("cube_1.png"_spr);
        spr->setRotation(55);
        spr->setShaderProgram(ShaderManager::get().getCosmicShader());

        auto btnSpr = CircleButtonSprite::create(spr, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
        btnSpr->setScale(.6f);
        auto btn = CCMenuItemExt::createSpriteExtra(btnSpr, [](auto) {
           openSettingsPopup(Mod::get(), true);
        });
        spr->setScale(.75);

        auto menu = getChildByID("left-button-menu");
        menu->addChild(btn);
        menu->updateLayout();
    }
};

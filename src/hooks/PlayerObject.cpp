#include "PlayerObject.hpp"
#include "GJBaseGameLayer.hpp"

void CosmicClonesPlayerObject::incrementJumps() {
    PlayerObject::incrementJumps();
    if (!PlayLayer::get()) return;
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(PlayLayer::get())->m_fields.self();
    if (!fields->m_enabled) return;
    if (this == PlayLayer::get()->m_player1) {
        fields->m_p1Jump = true;
    } else if (this == PlayLayer::get()->m_player2) {
        fields->m_p2Jump = true;
    }
}

void CosmicClonesPlayerObject::enablePlayerControls() {
    PlayerObject::enablePlayerControls();
    auto pl = PlayLayer::get();
    if (!pl) return;
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(pl)->m_fields.self();
    if (!fields->m_enabled) return;
    if (this == pl->m_player1) {
        fields->m_p1Frozen = false;
        fields->m_p1Immunity = 240;
    } else if (this == pl->m_player2) {
        fields->m_p2Frozen = false;
        fields->m_p2Immunity = 240;
    }
}

void CosmicClonesPlayerObject::disablePlayerControls() {
    PlayerObject::disablePlayerControls();
    auto pl = PlayLayer::get();
    if (!pl) return;
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(pl)->m_fields.self();
    if (!fields->m_enabled) return;
    if (this == pl->m_player1) {
        fields->m_p1Frozen = true;
    } else if (this == pl->m_player2) {
        fields->m_p2Frozen = true;
    }
}

#include "PlayerObject.hpp"
#include "GJBaseGameLayer.hpp"

void CosmicClonesPlayerObject::incrementJumps() {
    PlayerObject::incrementJumps();
    if (!m_gameLayer) return;
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(m_gameLayer)->m_fields.self();
    if (!fields->m_enabled) return;
    if (this == m_gameLayer->m_player1) {
        fields->m_p1Jump = true;
    } else if (this == m_gameLayer->m_player2) {
        fields->m_p2Jump = true;
    }
}

void CosmicClonesPlayerObject::enablePlayerControls() {
    PlayerObject::enablePlayerControls();
    if (!m_gameLayer) return;
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(m_gameLayer)->m_fields.self();
    if (!fields->m_enabled) return;
    if (this == m_gameLayer->m_player1) {
        fields->m_p1Frozen = false;
        fields->m_p1Immunity = 240;
    } else if (this == m_gameLayer->m_player2) {
        fields->m_p2Frozen = false;
        fields->m_p2Immunity = 240;
    }
}

void CosmicClonesPlayerObject::disablePlayerControls() {
    PlayerObject::disablePlayerControls();
    if (!m_gameLayer) return;
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(m_gameLayer)->m_fields.self();
    if (!fields->m_enabled) return;
    if (this == m_gameLayer->m_player1) {
        fields->m_p1Frozen = true;
    } else if (this == m_gameLayer->m_player2) {
        fields->m_p2Frozen = true;
    }
}

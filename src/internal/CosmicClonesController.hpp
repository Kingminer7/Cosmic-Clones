#pragma once

#include <Geode/utils/ZStringView.hpp>
#include "CosmicClone.hpp"
#include "../hooks/EditorUI.hpp"

struct CosmicClonesGJBGL;

class CosmicClonesController {
protected:
    CosmicClonesGJBGL* m_bgl = nullptr;

    std::vector<std::shared_ptr<CosmicClone>> m_clones;
    std::map<unsigned int, Snapshot> m_snapshots;
    std::vector<int> m_sfxIds;

    unsigned int m_startOffset = 0;

    void cleanup();

    bool m_stopped = true;

    CosmicClonesController(CosmicClonesGJBGL* bgl);
public:
    // i don't really wanna make getters/setters for the config options
    unsigned int m_count = 3;
    float m_initialDelay = 1.75;
    float m_delay = 1;
    bool m_damage = true;
    std::vector<geode::ZStringView> m_styles = {"Cosmic Mario\n(SMG 1)"};
    bool m_sfx = false;

    bool start();
    void stop(bool immediate = false);

    void loadConfigFromSettings();
    void loadConfigFromTrigger(const CosmicClonesTrigger* trigger);

    unsigned int getOffset() const;
    bool isStopped() const;
    std::vector<std::shared_ptr<CosmicClone>> getClones() const;

    void tick(int prog);
    void softReset(int prog);

    static CosmicClonesController* createWithSettings(CosmicClonesGJBGL* bgl);
    static CosmicClonesController* createFromTrigger(CosmicClonesGJBGL* bgl, CosmicClonesTrigger* trigger);
};

#include <cstdint>
#include <memory>
#include <vector>

#include "entity.h"
#include "map.h"

class EntityManager {
  public:
    EntityManager();
    ~EntityManager();

    int8_t indexOfPlayer;
    bool   isPlayerAlive { false };

    void                                        addEntity(std::unique_ptr<Entity> newEntity);
    void                                        cleanup();
    void                                        draw();
    const std::vector<std::unique_ptr<Entity>>& getEntities() const;
    void                                        reset();
    void                                        tick(const std::unique_ptr<Map>& map);

  private:
    std::vector<std::unique_ptr<Entity>> m_entities;

    void m_removeEntityByIndex(int8_t index);
    void m_ySortEntities();
};

#include "BaseGamer.hpp"
#include <cmath>

class RandomGamer : public Gamer{
public:
    void init (Field * f1, EnemyField * f2, ...) override;

    Coordinates getShipCoordinates(size_t size) override;
    ShotCoordinates make_shot() override;
};
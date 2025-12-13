//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ABSTRACTFACTORY_H
#define PACMANPROJECT_ABSTRACTFACTORY_H


namespace logic {
    class Fruit;
    class Coin;
    class Ghost;
    class PacManModel;

    class AbstractFactory {
    public:

        virtual PacManModel* createPacMan(float x, float y) = 0;

        virtual Ghost* createLockedGhost(float x, float y) = 0;
        virtual Ghost* createPredictiveGhost(float x, float y) = 0;
        virtual Ghost* createDirectChaseGhost(float x, float y) = 0;

        virtual Coin* createCoin(float x, float y) = 0;
        virtual Fruit* createFruit(float x, float y) = 0;

        virtual ~AbstractFactory() = default;
    };
}


#endif //PACMANPROJECT_ABSTRACTFACTORY_H

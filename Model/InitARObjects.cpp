//
// Created by Raphael Russo on 9/22/24.
// Util code for initializing AR Objects
//
#include "GameModel.h"

#include "../AR_Objs/Aruco/DevCards/KnightCard.h"
#include "../AR_Objs/Aruco/DevCards/MonopolyCard.h"
#include "../AR_Objs/Aruco/DevCards/RoadBuilding.h"
#include "../AR_Objs/Aruco/DevCards/VictoryPointCard.h"
#include "../AR_Objs/Aruco/DevCards/YearOfPlentyCard.h"

#include "../AR_Objs/Aruco/GamePieces/NonPlayerPieces/Robber.h"
#include "../AR_Objs/Aruco/GamePieces/PlayerPieces/City.h"
#include "../AR_Objs/Aruco/GamePieces/PlayerPieces/Road.h"
#include "../AR_Objs/Aruco/GamePieces/PlayerPieces/Settlement.h"

#include "../AR_Objs/Aruco/ResCards/BrickResCard.h"
#include "../AR_Objs/Aruco/ResCards/OreResCard.h"
#include "../AR_Objs/Aruco/ResCards/WheatResCard.h"
#include "../AR_Objs/Aruco/ResCards/WoodResCard.h"
#include "../AR_Objs/Aruco/ResCards/WoolResCard.h"

#include "../AR_Objs/Aruco/Tiles/BrickHex.h"
#include "../AR_Objs/Aruco/Tiles/OreHex.h"
#include "../AR_Objs/Aruco/Tiles/WheatHex.h"
#include "../AR_Objs/Aruco/Tiles/WoodHex.h"
#include "../AR_Objs/Aruco/Tiles/WoolHex.h"



void initGameObjs(GameModel &model) {
    KnightCard::registerKnightCard();
    MonopolyCard::registerMonopolyCard();
    RoadBuildingCard::registerRoadBuildingCard();
    VictoryPointCard::registerVictoryPointCard();
    YearOfPlentyCard::registerYearOfPlentyCard();

    Robber::registerRobber();

    City::registerCity();
    Road::registerRoad();
    Settlement::registerSettlement();

    BrickResCard::registerBrickResCard();
    OreResCard::registerOreResCard();
    WheatResCard::registerWheatResCard();
    WoodResCard::registerWoodResCard();
    WoolResCard::registerWoolResCard();

    BrickHex::registerBrickHex();
    OreHex::registerOreHex();
    WheatHex::registerWheatHex();
    WoodHex::registerWoodHex();
    WoolHex::registerWoolHex();

    for (int markerID = 1; markerID <= 147; ++markerID) {
        ARObject *arObject = ARObjectFactory::createARObject(markerID);
        if (arObject) {
            model.addObject(markerID, arObject);
        }
    }
}

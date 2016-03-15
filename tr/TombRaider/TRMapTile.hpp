//
//  TRMapTile.hpp
//  TombRaider
//
//  Created by 張正昊 on 13/2/2016.
//  Copyright © 2016 hahaschool. All rights reserved.
//

#ifndef TRMapTile_hpp
#define TRMapTile_hpp

#include <stdio.h>
#include "TRObject.hpp"
#include "TRPathFinder.hpp"
#include "TRBgm.h"
enum TRMapTileType{
    TRMapTileTypeGround = 0,
    TRMapTileTypeWall = 1,
    TRMapTileTypeDoor = 2,
    TRMapTileTypeTeleport = 3,
    TRMapTileTypeCatchable = 4
};

class TRMapTile : public TRObject{
public:
    TRMapTile();
    bool isPassBy();
    void setPassBy(bool flg);

    bool isCatchable();
    void setCatchable(bool flgCatchable);

    void setObjectBeCatched(int j,int i,TRPathFinderMatrix isPass);
private:
    //地图块是否可穿越
    bool flgPassBy;
    //可被捡起
    bool flgCatchable;

    TRPathFinder *gPathFinder;
    TRBgm *gBgm;
};

#endif /* TRMapTile_hpp */

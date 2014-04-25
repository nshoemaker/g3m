package org.glob3.mobile.generated; 
//
//  TileImageProvider.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 4/18/14.
//
//


//
//  TileImageProvider.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 4/18/14.
//
//



//class Tile;
//class TileImageListener;
//class Vector2I;


public abstract class TileImageProvider extends RCObject
{
  public void dispose()
  {
    super.dispose();
  }


  public abstract TileImageContribution contribution(Tile tile);

  public abstract void create(Tile tile, TileImageContribution contribution, Vector2I resolution, long tileDownloadPriority, boolean logDownloadActivity, TileImageListener listener, boolean deleteListener);

  public abstract void cancel(Tile tile);

}
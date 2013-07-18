package org.glob3.mobile.generated; 
//
//  BufferElevationData.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 2/23/13.
//
//

//
//  BufferElevationData.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 2/23/13.
//
//



public abstract class BufferElevationData extends ElevationData
{
  protected final int _bufferSize;

  protected abstract double getValueInBufferAt(int index);

  public BufferElevationData(Sector sector, Vector2I extent, Sector realSector, Vector2I realExtent, int bufferSize)
  //_realResolution(realSector._deltaLatitude.div(realExtent._y),
  //                realSector._deltaLongitude.div(realExtent._x))
  {
     super(sector, extent);
     _bufferSize = bufferSize;
  
  }

  public void dispose()
  {

  }

//  const Geodetic2D getRealResolution() const {
//    return _realResolution;
//  }

  public final double getElevationAt(int x, int y)
  {
    final int index = ((_height-1-y) * _width) + x;
  
  //  if ( (index < 0) || (index >= _bufferSize) ) {
  //    printf("break point on me\n");
  //    return IMathUtils::instance()->NanD();
  //  }
  
    return getValueInBufferAt(index);
  }

}
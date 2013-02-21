package org.glob3.mobile.generated; 
//
//  Sector.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 22/06/12.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.
//

//
//  Sector.hpp
//  G3MiOSSDK
//
//  Created by Agustín Trujillo Pino on 12/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//





public class Sector
{

  private final Geodetic2D _lower ;
  private final Geodetic2D _upper ;
  private final Geodetic2D _center ;

  private final Angle _deltaLatitude ;
  private final Angle _deltaLongitude ;




  public void dispose()
  {
  }

  public Sector(Geodetic2D lower, Geodetic2D upper)
  {
     _lower = new Geodetic2D(lower);
     _upper = new Geodetic2D(upper);
     _deltaLatitude = new Angle(upper.latitude().sub(lower.latitude()));
     _deltaLongitude = new Angle(upper.longitude().sub(lower.longitude()));
     _center = new Geodetic2D(Angle.midAngle(lower.latitude(), upper.latitude()), Angle.midAngle(lower.longitude(), upper.longitude()));
  }


  public Sector(Sector sector)
  {
     _lower = new Geodetic2D(sector._lower);
     _upper = new Geodetic2D(sector._upper);
     _deltaLatitude = new Angle(sector._deltaLatitude);
     _deltaLongitude = new Angle(sector._deltaLongitude);
     _center = new Geodetic2D(sector._center);
  }

  public static Sector fromDegrees(double minLat, double minLon, double maxLat, double maxLon)
  {
    final Geodetic2D lower = new Geodetic2D(Angle.fromDegrees(minLat), Angle.fromDegrees(minLon));
    final Geodetic2D upper = new Geodetic2D(Angle.fromDegrees(maxLat), Angle.fromDegrees(maxLon));

    return new Sector(lower, upper);
  }

  public final Vector2D getScaleFactor(Sector that)
  {
    final double u = _deltaLatitude.div(that._deltaLatitude);
    final double v = _deltaLongitude.div(that._deltaLongitude);
    return new Vector2D(u, v);
  }

  public final Vector2D getTranslationFactor(Sector that)
  {
    final double diff = _deltaLongitude.div(that._deltaLongitude);
    final Vector2D uv = that.getUVCoordinates(_lower);

    return new Vector2D(uv._x, uv._y - diff);
  }

  public final boolean fullContains(Sector s)
  {
    return contains(s.upper()) && contains(s.lower());
  }

  public final Sector intersection(Sector that)
  {
    final Angle lowLat = Angle.max(lower().latitude(), that.lower().latitude());
    final Angle lowLon = Angle.max(lower().longitude(), that.lower().longitude());
    final Geodetic2D low = new Geodetic2D(lowLat, lowLon);
  
    final Angle upLat = Angle.min(upper().latitude(), that.upper().latitude());
    final Angle upLon = Angle.min(upper().longitude(), that.upper().longitude());
    final Geodetic2D up = new Geodetic2D(upLat, upLon);
  
    return new Sector(low, up);
  }

  public final Sector mergedWith(Sector that)
  {
    final Angle lowLat = Angle.min(lower().latitude(), that.lower().latitude());
    final Angle lowLon = Angle.min(lower().longitude(), that.lower().longitude());
    final Geodetic2D low = new Geodetic2D(lowLat, lowLon);
  
    final Angle upLat = Angle.max(upper().latitude(), that.upper().latitude());
    final Angle upLon = Angle.max(upper().longitude(), that.upper().longitude());
    final Geodetic2D up = new Geodetic2D(upLat, upLon);
  
    return new Sector(low, up);
  }

  public static Sector fullSphere()
  {
    return new Sector(new Geodetic2D(Angle.fromDegrees(-90), Angle.fromDegrees(-180)), new Geodetic2D(Angle.fromDegrees(90), Angle.fromDegrees(180)));
  }

  public final Geodetic2D lower()
  {
    return _lower;
  }

  public final Angle lowerLatitude()
  {
    return _lower.latitude();
  }

  public final Angle lowerLongitude()
  {
    return _lower.longitude();
  }

  public final Geodetic2D upper()
  {
    return _upper;
  }

  public final Angle upperLatitude()
  {
    return _upper.latitude();
  }

  public final Angle upperLongitude()
  {
    return _upper.longitude();
  }

  public final boolean contains(Geodetic2D position)
  {
    return position.isBetween(_lower, _upper);
  }

  public final boolean contains(Geodetic3D position)
  {
    return contains(position.asGeodetic2D());
  }

  public final boolean touchesWith(Sector that)
  {
    // from Real-Time Collision Detection - Christer Ericson
    //   page 79
  
    // Exit with no intersection if separated along an axis
    if (_upper.latitude().lowerThan(that._lower.latitude()) || _lower.latitude().greaterThan(that._upper.latitude()))
    {
      return false;
    }
    if (_upper.longitude().lowerThan(that._lower.longitude()) || _lower.longitude().greaterThan(that._upper.longitude()))
    {
      return false;
    }
  
    // Overlapping on all axes means Sectors are intersecting
    return true;
  }

  public final Angle getDeltaLatitude()
  {
    return _deltaLatitude;
  }

  public final Angle getDeltaLongitude()
  {
    return _deltaLongitude;
  }

  public final Geodetic2D getSW()
  {
    return _lower;
  }

  public final Geodetic2D getNE()
  {
    return _upper;
  }

  public final Geodetic2D getNW()
  {
    return new Geodetic2D(_upper.latitude(), _lower.longitude());
  }

  public final Geodetic2D getSE()
  {
    return new Geodetic2D(_lower.latitude(), _upper.longitude());
  }

  public final Geodetic2D getCenter()
  {
    return _center;
  }

  // (u,v) are similar to texture coordinates inside the Sector
  // (u,v)=(0,0) in NW point, and (1,1) in SE point

  // (u,v) are similar to texture coordinates inside the Sector
  // (u,v)=(0,0) in NW point, and (1,1) in SE point
  public final Geodetic2D getInnerPoint(double u, double v)
  {
    return new Geodetic2D(Angle.lerp(_lower.latitude(), _upper.latitude(), (float)(1.0-v)), Angle.lerp(_lower.longitude(), _upper.longitude(), (float) u));
  }

  public final Vector2D getUVCoordinates(Geodetic2D point)
  {
    return getUVCoordinates(point.latitude(), point.longitude());
  }

  public final Vector2D getUVCoordinates(Angle latitude, Angle longitude)
  {
    final double u = longitude.sub(_lower.longitude()).div(getDeltaLongitude());
    final double v = _upper.latitude().sub(latitude).div(getDeltaLatitude());
    return new Vector2D(u, v);
  }

  public final boolean isBackOriented(G3MRenderContext rc)
  {
    final Camera camera = rc.getCurrentCamera();
    final Planet planet = rc.getPlanet();
  
    // compute sector point nearest to centerPoint
    final Geodetic2D center = camera.getGeodeticCenterOfView().asGeodetic2D();
    final Geodetic2D point = getClosestPoint(center);
  
    // compute angle between normals
    final Vector3D normal = planet.geodeticSurfaceNormal(point);
    final Vector3D view = camera.getViewDirection().times(-1);
    final double dot = normal.dot(view);
  
    return (dot < 0) ? true : false;
  }

  public final Geodetic2D getClosestPoint(Geodetic2D pos)
  {
    // if pos is included, return pos
    if (contains(pos))
       return pos;
  
    // test longitude
    Geodetic2D center = getCenter();
    double lon = pos.longitude()._degrees;
    double centerLon = center.longitude()._degrees;
    double oppLon1 = centerLon - 180;
    double oppLon2 = centerLon + 180;
    if (lon<oppLon1)
      lon+=360;
    if (lon>oppLon2)
      lon-=360;
    double minLon = _lower.longitude()._degrees;
    double maxLon = _upper.longitude()._degrees;
    //bool insideLon    = true;
    if (lon < minLon)
    {
      lon = minLon;
      //insideLon = false;
    }
    if (lon > maxLon)
    {
      lon = maxLon;
      //insideLon = false;
    }
  
    // test latitude
    double lat = pos.latitude()._degrees;
    double minLat = _lower.latitude()._degrees;
    double maxLat = _upper.latitude()._degrees;
    //bool insideLat    = true;
    if (lat < minLat)
    {
      lat = minLat;
      //insideLat = false;
    }
    if (lat > maxLat)
    {
      lat = maxLat;
      //insideLat = false;
    }
  
    // here we have to handle the case where sectos is close to the pole,
    // and the latitude of the other point must be seen from the other side
  
  
    return new Geodetic2D(Angle.fromDegrees(lat), Angle.fromDegrees(lon));
  
  
  
  /*
    const Angle lat = pos.latitude().nearestAngleInInterval(_lower.latitude(), _upper.latitude());
    const Angle lon = pos.longitude().nearestAngleInInterval(_lower.longitude(), _upper.longitude());
    return Geodetic2D(lat, lon);*/
  }

//C++ TO JAVA CONVERTER TODO TASK: The implementation of the following method could not be found:
//  Geodetic2D getApproximatedClosestPoint(Geodetic2D pos);

  public final String description()
  {
    IStringBuilder isb = IStringBuilder.newStringBuilder();
    isb.addString("(Sector ");
    isb.addString(_lower.description());
    isb.addString(" - ");
    isb.addString(_upper.description());
    isb.addString(")");
    final String s = isb.getString();
    if (isb != null)
       isb.dispose();
    return s;
  }

  public final Sector shrinkedByPercentP(float percent)
  {
    Angle deltaLatitude = _deltaLatitude.times(percent).div(2);
    Angle deltaLongitude = _deltaLongitude.times(percent).div(2);

    Geodetic2D delta = new Geodetic2D(deltaLatitude, deltaLongitude);

    return new Sector(_lower.add(delta), _upper.sub(delta));
  }

  public final boolean isEqualsTo(Sector that)
  {
    return _lower.isEqualsTo(that._lower) && _upper.isEqualsTo(that._upper);
  }

}
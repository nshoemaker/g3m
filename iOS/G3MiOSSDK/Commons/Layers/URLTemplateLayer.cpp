//
//  URLTemplateLayer.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 9/29/13.
//
//

#include "URLTemplateLayer.hpp"

#include "LayerTilesRenderParameters.hpp"
#include "LayerCondition.hpp"
#include "Tile.hpp"
#include "IStringUtils.hpp"
#include "IMathUtils.hpp"
#include "MercatorUtils.hpp"
#include "LevelTileCondition.hpp"
#include "TimeInterval.hpp"
#include "RenderState.hpp"
#include "URL.hpp"

URLTemplateLayer::URLTemplateLayer(const std::string&                urlTemplate,
                                   const Sector&                     dataSector,
                                   const bool                        isTransparent,
                                   const TimeInterval&               timeToCache,
                                   const bool                        readExpired,
                                   const LayerCondition*             condition,
                                   const LayerTilesRenderParameters* parameters,
                                   float                             transparency,
                                   std::vector<const Info*>*   layerInfo) :
RasterLayer(timeToCache,
            readExpired,
            parameters,
            transparency,
            condition,
            layerInfo),
_urlTemplate(urlTemplate),
_dataSector(dataSector),
_isTransparent(isTransparent),
_su(NULL),
_mu(NULL),
_tiled(true)
{
}

URLTemplateLayer* URLTemplateLayer::newMercator(const std::string&    urlTemplate,
                                                const Sector&         dataSector,
                                                const bool            isTransparent,
                                                const int             firstLevel,
                                                const int             maxLevel,
                                                const TimeInterval&   timeToCache,
                                                const bool            readExpired,
                                                const float                     transparency,
                                                const LayerCondition*           condition,
                                                std::vector<const Info*>* layerInfo) {
  return new URLTemplateLayer(urlTemplate,
                              dataSector,
                              isTransparent,
                              timeToCache,
                              readExpired,
                              (condition == NULL) ? new LevelTileCondition(firstLevel, maxLevel) : condition,
                              LayerTilesRenderParameters::createDefaultMercator(2,
                                                                                maxLevel),
                              transparency,
                              layerInfo);
}

URLTemplateLayer* URLTemplateLayer::newWGS84(const std::string&    urlTemplate,
                                             const Sector&         dataSector,
                                             const bool            isTransparent,
                                             const int             firstLevel,
                                             const int             maxLevel,
                                             const TimeInterval&   timeToCache,
                                             const bool            readExpired,
                                             const LayerCondition* condition,
                                             const float           transparency,
                                             std::vector<const Info*>*  layerInfo) {
  return new URLTemplateLayer(urlTemplate,
                              dataSector,
                              isTransparent,
                              timeToCache,
                              readExpired,
                              (condition == NULL) ? new LevelTileCondition(firstLevel, maxLevel) : condition,
                              LayerTilesRenderParameters::createDefaultWGS84(dataSector, firstLevel, maxLevel),
                              transparency,
                              layerInfo);
}

bool URLTemplateLayer::rawIsEquals(const Layer* that) const {
  URLTemplateLayer* t = (URLTemplateLayer*) that;
  
  if (_isTransparent != t->_isTransparent) {
    return false;
  }
  
  if (_urlTemplate != t->_urlTemplate) {
    return false;
  }
  
  return (_dataSector.isEquals(t->_dataSector));
}

const std::string URLTemplateLayer::description() const {
  return "[URLTemplateLayer urlTemplate=\"" + _urlTemplate + "\"]";
}

URLTemplateLayer* URLTemplateLayer::copy() const {
  return new URLTemplateLayer(_urlTemplate,
                              _dataSector,
                              _isTransparent,
                              _timeToCache,
                              _readExpired,
                              (_condition == NULL) ? NULL : _condition->copy(),
                              _parameters->copy(),
                              _transparency,
                              _layerInfo);
}

URL URLTemplateLayer::getFeatureInfoURL(const Geodetic2D& position,
                                        const Sector& sector) const {
  return URL();
}

const std::string URLTemplateLayer::getPath(const LayerTilesRenderParameters* layerTilesRenderParameters,
                                            const Tile* tile,
                                            const Sector& sector) const {
  
  if (_mu == NULL) {
    _mu = IMathUtils::instance();
  }
  
  if (_su == NULL) {
    _su = IStringUtils::instance();
  }
  
  const Vector2I tileTextureResolution = _parameters->_tileTextureResolution;
  
  const int level   = tile->_level;
  const int column  = tile->_column;
  const int numRows = (int) (layerTilesRenderParameters->_topSectorSplitsByLatitude * _mu->pow(2.0, level));
  const int row     = numRows - tile->_row - 1;
  
  const double north = MercatorUtils::latitudeToMeters( sector._upper._latitude );
  const double south = MercatorUtils::latitudeToMeters( sector._lower._latitude );
  const double east  = MercatorUtils::longitudeToMeters( sector._upper._longitude );
  const double west  = MercatorUtils::longitudeToMeters( sector._lower._longitude );
  
  std::string path = _urlTemplate;
  path = _su->replaceSubstring(path, "{width}",          _su->toString( tileTextureResolution._x          ) );
  path = _su->replaceSubstring(path, "{height}",         _su->toString( tileTextureResolution._y          ) );
  path = _su->replaceSubstring(path, "{x}",              _su->toString( column                            ) );
  path = _su->replaceSubstring(path, "{y}",              _su->toString( row                               ) );
  path = _su->replaceSubstring(path, "{y2}",             _su->toString( tile->_row                        ) );
  path = _su->replaceSubstring(path, "{level}",          _su->toString( level                             ) );
  path = _su->replaceSubstring(path, "{lowerLatitude}",  _su->toString( sector._lower._latitude._degrees  ) );
  path = _su->replaceSubstring(path, "{lowerLongitude}", _su->toString( sector._lower._longitude._degrees ) );
  path = _su->replaceSubstring(path, "{upperLatitude}",  _su->toString( sector._upper._latitude._degrees  ) );
  path = _su->replaceSubstring(path, "{upperLongitude}", _su->toString( sector._upper._longitude._degrees ) );
  path = _su->replaceSubstring(path, "{north}",          _su->toString( north                             ) );
  path = _su->replaceSubstring(path, "{south}",          _su->toString( south                             ) );
  path = _su->replaceSubstring(path, "{west}",           _su->toString( west                              ) );
  path = _su->replaceSubstring(path, "{east}",           _su->toString( east                              ) );
  
  return path;
}

const URL URLTemplateLayer::createURL(const Tile* tile) const {
  
  if (_mu == NULL) {
    _mu = IMathUtils::instance();
  }
  
  if (_su == NULL) {
    _su = IStringUtils::instance();
  }
  
  const Sector sector = tile->_sector;
  
  const Vector2I tileTextureResolution = _parameters->_tileTextureResolution;
  
  const int level   = tile->_level;
  const int column  = tile->_column;
  const int numRows = (int) (_parameters->_topSectorSplitsByLatitude * _mu->pow(2.0, level));
  const int row     = numRows - tile->_row - 1;
  
  const double north = MercatorUtils::latitudeToMeters( sector._upper._latitude );
  const double south = MercatorUtils::latitudeToMeters( sector._lower._latitude );
  const double east  = MercatorUtils::longitudeToMeters( sector._upper._longitude );
  const double west  = MercatorUtils::longitudeToMeters( sector._lower._longitude );
  
  std::string path = _urlTemplate;
  path = _su->replaceSubstring(path, "{width}",          _su->toString( tileTextureResolution._x          ) );
  path = _su->replaceSubstring(path, "{height}",         _su->toString( tileTextureResolution._y          ) );
  path = _su->replaceSubstring(path, "{x}",              _su->toString( column                            ) );
  path = _su->replaceSubstring(path, "{y}",              _su->toString( row                               ) );
  path = _su->replaceSubstring(path, "{y2}",             _su->toString( tile->_row                        ) );
  path = _su->replaceSubstring(path, "{level}",          _su->toString( level                             ) );
  path = _su->replaceSubstring(path, "{lowerLatitude}",  _su->toString( sector._lower._latitude._degrees  ) );
  path = _su->replaceSubstring(path, "{lowerLongitude}", _su->toString( sector._lower._longitude._degrees ) );
  path = _su->replaceSubstring(path, "{upperLatitude}",  _su->toString( sector._upper._latitude._degrees  ) );
  path = _su->replaceSubstring(path, "{upperLongitude}", _su->toString( sector._upper._longitude._degrees ) );
  path = _su->replaceSubstring(path, "{north}",          _su->toString( north                             ) );
  path = _su->replaceSubstring(path, "{south}",          _su->toString( south                             ) );
  path = _su->replaceSubstring(path, "{west}",           _su->toString( west                              ) );
  path = _su->replaceSubstring(path, "{east}",           _su->toString( east                              ) );
  
  return URL(path, false);
}

RenderState URLTemplateLayer::getRenderState() {
  _errors.clear();
  if (_urlTemplate.compare("") == 0) {
    _errors.push_back("Missing layer parameter: urlTemplate");
  }
  
  if (_errors.size() > 0) {
    return RenderState::error(_errors);
  }
  return RenderState::ready();
}

const TileImageContribution* URLTemplateLayer::rawContribution(const Tile* tile) const {
  const Tile* tileP = getParentTileOfSuitableLevel(tile);
  if (tileP == NULL) {
    return NULL;
  }
  
  const Sector requestedImageSector = tileP->_sector;
  
  if (!_dataSector.touchesWith(requestedImageSector)) {
    return NULL;
  }
  
  if (tile == tileP && ( _dataSector.fullContains(requestedImageSector) || _tiled )) {
    return ((_isTransparent || (_transparency < 1))
            ? TileImageContribution::fullCoverageTransparent(_transparency)
            : TileImageContribution::fullCoverageOpaque());
  }
  
  return  ((_isTransparent || (_transparency < 1))
           ? TileImageContribution::partialCoverageTransparent(requestedImageSector, _transparency)
           : TileImageContribution::partialCoverageOpaque(requestedImageSector));
}

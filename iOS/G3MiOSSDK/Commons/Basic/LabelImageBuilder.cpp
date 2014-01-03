//
//  LabelImageBuilder.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 1/3/14.
//
//

#include "LabelImageBuilder.hpp"

#include "Context.hpp"
#include "IFactory.hpp"
#include "ICanvas.hpp"
#include "IMathUtils.hpp"
#include "IStringUtils.hpp"
#include "IImageListener.hpp"
#include "IImageBuilderListener.hpp"


const std::string LabelImageBuilder::getImageName() {
  return (_text + "/" +
          _font.description() +  "/" +
          IStringUtils::instance()->toString(_margin));
}


class LabelImageBuilder_ImageListener : public IImageListener {
private:
  IImageBuilderListener* _listener;
  bool                   _deleteListener;
  const std::string      _imageName;

public:
  LabelImageBuilder_ImageListener(IImageBuilderListener* listener,
                                  bool deleteListener,
                                  const std::string& imageName) :
  _listener(listener),
  _deleteListener(deleteListener),
  _imageName(imageName)
  {
  }


  void imageCreated(const IImage* image) {
    _listener->imageCreated(image, _imageName);
    if (_deleteListener) {
      delete _listener;
    }
    _listener = NULL;
  }

  ~LabelImageBuilder_ImageListener() {
    delete _listener;
  }
};

void LabelImageBuilder::build(const G3MContext* context,
                              IImageBuilderListener* listener,
                              bool deleteListener) {

  const IFactory* factory = context->getFactory();

  ICanvas* canvas = factory->createCanvas();

  canvas->setFont(_font);

  const Vector2F textExtent = canvas->textExtent(_text);

  const IMathUtils* mu = context->getMathUtils();

  canvas->initialize(mu->round(textExtent._x + _margin*2),
                     mu->round(textExtent._y + _margin*2));

  canvas->setFillColor(_color);
  canvas->fillText(_text, _margin, _margin);

  canvas->createImage(new LabelImageBuilder_ImageListener(listener, deleteListener,
                                                          getImageName()),
                      true);
  
  delete canvas;
}
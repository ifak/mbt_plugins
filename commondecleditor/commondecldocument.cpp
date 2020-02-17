#include "commondecldocument.hpp"

#include "commondecloutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace model::base;
using namespace commondecleditor;

class CommonDeclDocument::Private
{
  friend class CommonDeclDocument;

  CommonDeclOutlineModel* _commondeclOutlineModel;
  BaseModel*          _commondeclDeclModel;

  Private()
    : _commondeclOutlineModel(0),
      _commondeclDeclModel(0)
  {}
};

CommonDeclDocument::CommonDeclDocument(QObject *parent)
  : TextEditor::BaseTextDocument(parent),
    _d(new Private)
{
  this->_d->_commondeclOutlineModel = new CommonDeclOutlineModel(this);
  this->_d->_commondeclDeclModel = new BaseModel(this);
}

CommonDeclDocument::~CommonDeclDocument()
{}

CommonDeclOutlineModel* CommonDeclDocument::commondeclOutlineModel()
{
  return this->_d->_commondeclOutlineModel;
}

BaseModel* CommonDeclDocument::commondeclDeclModel()
{
  return this->_d->_commondeclDeclModel;
}

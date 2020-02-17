#include "isrdldocument.hpp"

#include "isrdloutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace model::isrdl;
using namespace isrdleditor;

class IsrdlDocument::Private
{
  friend class IsrdlDocument;

  IsrdlOutlineModel* _isrdlOutlineModel;
  IsrdlModel*          _isrdlDeclModel;

  Private()
    : _isrdlOutlineModel(0),
      _isrdlDeclModel(0)
  {}
};

IsrdlDocument::IsrdlDocument(QObject *parent)
  : TextEditor::BaseTextDocument(parent),
    _d(new Private)
{
  this->_d->_isrdlOutlineModel = new IsrdlOutlineModel(this);
  this->_d->_isrdlDeclModel = new IsrdlModel(this);
}

IsrdlDocument::~IsrdlDocument()
{}

IsrdlOutlineModel* IsrdlDocument::isrdlOutlineModel()
{
  return this->_d->_isrdlOutlineModel;
}

IsrdlModel* IsrdlDocument::isrdlDeclModel()
{
  return this->_d->_isrdlDeclModel;
}

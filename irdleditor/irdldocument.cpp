#include "irdldocument.hpp"

#include "irdloutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace model::irdl;
using namespace irdleditor;

class IrdlDocument::Private
{
  friend class IrdlDocument;

  IrdlOutlineModel* _irdlOutlineModel;
  RequirementsModel*          _irdlDeclModel;

  Private()
    : _irdlOutlineModel(0),
      _irdlDeclModel(0)
  {}
};

IrdlDocument::IrdlDocument(QObject *parent)
  : TextEditor::BaseTextDocument(parent),
    _d(new Private)
{
  this->_d->_irdlOutlineModel = new IrdlOutlineModel(this);
  this->_d->_irdlDeclModel = new RequirementsModel(this);
}

IrdlDocument::~IrdlDocument()
{}

IrdlOutlineModel* IrdlDocument::irdlOutlineModel()
{
  return this->_d->_irdlOutlineModel;
}

RequirementsModel* IrdlDocument::irdlDeclModel()
{
  return this->_d->_irdlDeclModel;
}

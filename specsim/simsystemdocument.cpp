#include "simsystemdocument.hpp"

#include "simsystemoutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace modsim;
using namespace specsim;

class SimSystemDocument::Private
{
  friend class SimSystemDocument;

  SimSystemOutlineModelPtr _simSystemOutlineModel;
  SimSystemModel*          _simSystemDeclModel;

  Private()
    : _simSystemOutlineModel(new SimSystemOutlineModel()),
      _simSystemDeclModel(0)
  {}
};

SimSystemDocument::SimSystemDocument(QObject *parent)
  : TextEditor::BaseTextDocument(parent),
    _d(new Private)
{
  this->_d->_simSystemDeclModel = new SimSystemModel;
}

SimSystemDocument::~SimSystemDocument()
{}

const SimSystemOutlineModelPtr& SimSystemDocument::simsystemOutlineModel()
{
  return this->_d->_simSystemOutlineModel;
}

SimSystemModel* SimSystemDocument::simsystemDeclModel()
{
  return this->_d->_simSystemDeclModel;
}

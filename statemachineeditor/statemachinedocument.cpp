#include "statemachinedocument.hpp"

#include "statemachineoutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace model::statemachine;
using namespace statemachineeditor;

class StateMachineDocument::Private
{
  friend class StateMachineDocument;

  StateMachineOutlineModelPtr _stateMachineOutlineModel;
  StateMachineModel*          _statemachineDeclModel;

  Private()
    : _stateMachineOutlineModel(new StateMachineOutlineModel()),
      _statemachineDeclModel(0)
  {}
};

StateMachineDocument::StateMachineDocument(QObject *parent)
  : TextEditor::BaseTextDocument(parent),
    _d(new Private)
{
  this->_d->_statemachineDeclModel = new StateMachineModel(this);
}

StateMachineDocument::~StateMachineDocument()
{}

const StateMachineOutlineModelPtr& StateMachineDocument::statemachineOutlineModel()
{
  return this->_d->_stateMachineOutlineModel;
}

StateMachineModel* StateMachineDocument::statemachineDeclModel()
{
  return this->_d->_statemachineDeclModel;
}

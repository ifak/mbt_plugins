#include "spenatdocument.hpp"

#include "spenatoutlinemodel.hpp"

#include <mobata/memory_leak_start.hpp>

using namespace model::spenat;
using namespace spenateditor;

class SpenatDocument::Private
{
  friend class SpenatDocument;

  SpenatOutlineModelPtr _spenatOutlineModel;
  SpenatDeclModel*            _spenatDeclModel;

  Private()
    : _spenatOutlineModel(new SpenatOutlineModel()),
      _spenatDeclModel(nullptr)
  {}
};

SpenatDocument::SpenatDocument(QObject *parent)
  : TextEditor::BaseTextDocument(parent),
    _d(new Private)
{
  this->_d->_spenatDeclModel = new SpenatDeclModel(this);
}

SpenatDocument::~SpenatDocument()
{}

const SpenatOutlineModelPtr& SpenatDocument::spenatOutlineModel()
{
  return this->_d->_spenatOutlineModel;
}

SpenatDeclModel* SpenatDocument::spenatDeclModel()
{
  return this->_d->_spenatDeclModel;
}

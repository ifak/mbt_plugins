#include "mbtptablemodel.hpp"
#include <QDebug>

namespace mbtp {

MbtpTableModel::MbtpTableModel(int size) : QAbstractTableModel (nullptr)
{
  this->_size = size;
}

MbtpTableModel::~MbtpTableModel(){

}

void MbtpTableModel::pushBack(QVector<QVariant> data){
    this->_data.append(data);
}

int MbtpTableModel::rowCount(const QModelIndex& index) const
{
  Q_UNUSED(index);
  return this->_data.size();
}

int MbtpTableModel::columnCount(const QModelIndex& index) const
{
  Q_UNUSED(index);
  return this->_size;
}

QVariant MbtpTableModel::data(const QModelIndex &index, int role) const
{
  switch(role)
  {
  case OneRole:
    return _data[index.row()][0];
  case TwoRole:
    return _data[index.row()][1];
  case ThreeRole:
    return _data[index.row()][2];
  case FourRole:
    return _data[index.row()][3];
  case FiveRole:
    return _data[index.row()][4];
  case SixRole:
    return _data[index.row()][5];
  }
  return QVariant();
}

QHash<int, QByteArray> MbtpTableModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[OneRole] = "one";
  roles[TwoRole] = "two";
  roles[ThreeRole] = "three";
  roles[FourRole] = "four";
  roles[FiveRole] = "five";
  roles[SixRole] = "six";
  return roles;
}

}

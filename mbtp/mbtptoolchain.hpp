/*
 * This file is part of mbt_plugins.
 *
 * mbt_plugins is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * mbt_plugins is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with mbt_plugins.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MBTP_MBTPTOOLCHAIN_HPP
#define MBTP_MBTPTOOLCHAIN_HPP

#include "mbtp_global.hpp"

#include <projectexplorer/baseprojecttoolchain.hpp>

namespace mbtp {

class MBTPSHARED_EXPORT MbtpToolChain
    : public ProjectExplorer::BaseProjectToolChain
{
public:
  MbtpToolChain();
  MbtpToolChain(bool autodetect);
  virtual ~MbtpToolChain();
};

class MBTPSHARED_EXPORT MbtpToolChainFactory
    : public ProjectExplorer::BaseProjectToolChainFactory
{
  Q_OBJECT

public:
  MbtpToolChainFactory();
  virtual ~MbtpToolChainFactory();

public:/// relevant virtual implementations
  MbtpToolChain* create();
  MbtpToolChain* restore(const QVariantMap& data);
};

} /// end namespace mbtp

#endif // MBTP_MBTPTOOLCHAIN_HPP

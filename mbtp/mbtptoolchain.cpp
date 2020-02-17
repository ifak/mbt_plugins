#include "mbtptoolchain.hpp"

#include "mbtpconstants.hpp"

#include <mobata/memory_leak_start.hpp>

namespace mbtp {

// --------------------------------------------------------------------------
// MbtpToolChain
// --------------------------------------------------------------------------

MbtpToolChain::MbtpToolChain()
  : ProjectExplorer::BaseProjectToolChain(QLatin1String(Constants::MBTP_TOOLCHAIN),
                                          QLatin1String(Constants::MBTP_PROJECT),
                                          QStringLiteral("Mbtp Project"),
                                          QStringLiteral("Test Prioritization Tool Chain"),
                                          true)
{}

MbtpToolChain::MbtpToolChain(bool autodetect)
  : ProjectExplorer::BaseProjectToolChain(QLatin1String(Constants::MBTP_TOOLCHAIN),
                                          QLatin1String(Constants::MBTP_PROJECT),
                                          QStringLiteral("Mbtp Project"),
                                          QStringLiteral("Test Prioritization Tool Chain"),
                                          autodetect)
{}

MbtpToolChain::~MbtpToolChain()
{}

//// --------------------------------------------------------------------------
//// MbtpToolChainFactory
//// --------------------------------------------------------------------------

MbtpToolChainFactory::MbtpToolChainFactory()
  : ProjectExplorer::BaseProjectToolChainFactory(QLatin1String(Constants::MBTP_TOOLCHAIN),
                                                 tr("Test Prioritization Tool Chain"))
{}

MbtpToolChainFactory::~MbtpToolChainFactory()
{}

MbtpToolChain* MbtpToolChainFactory::create()
{
  return new MbtpToolChain(true);
}

MbtpToolChain* MbtpToolChainFactory::restore(const QVariantMap &data)
{
  MbtpToolChain* mbtp_tc = new MbtpToolChain(false);
  if (mbtp_tc->fromMap(data))
    return mbtp_tc;
  else
  {
    delete mbtp_tc;
    return nullptr;
  }
}

} // namespace mbtp

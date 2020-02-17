#include "mbtprojecttoolchain.hpp"

#include "mbtprojectconstants.hpp"

#include <mobata/memory_leak_start.hpp>

namespace mbtproject {

// --------------------------------------------------------------------------
// MbtProjectToolChain
// --------------------------------------------------------------------------

MbtProjectToolChain::MbtProjectToolChain()
  : ProjectExplorer::BaseProjectToolChain(QLatin1String(Constants::MBT_TOOLCHAIN),
                                          QLatin1String(Constants::MBT_PROJECT),
                                          QStringLiteral("MBT Project"),
                                          QStringLiteral("Test Generation Tool Chain"),
                                          true)
{}

MbtProjectToolChain::MbtProjectToolChain(bool autodetect)
  : ProjectExplorer::BaseProjectToolChain(QLatin1String(Constants::MBT_TOOLCHAIN),
                                          QLatin1String(Constants::MBT_PROJECT),
                                          QStringLiteral("MBT Project"),
                                          QStringLiteral("Test Generation Tool Chain"),
                                          autodetect)
{}

MbtProjectToolChain::~MbtProjectToolChain()
{}

//// --------------------------------------------------------------------------
//// MbtProjectToolChainFactory
//// --------------------------------------------------------------------------

MbtProjectToolChainFactory::MbtProjectToolChainFactory()
  : ProjectExplorer::BaseProjectToolChainFactory(QLatin1String(Constants::MBT_TOOLCHAIN),
                                                 tr("Test Generation Tool Chain"))
{}

MbtProjectToolChainFactory::~MbtProjectToolChainFactory()
{}

MbtProjectToolChain* MbtProjectToolChainFactory::create()
{
  return new MbtProjectToolChain(true);
}

MbtProjectToolChain* MbtProjectToolChainFactory::restore(const QVariantMap &data)
{
  MbtProjectToolChain* mbt_tc = new MbtProjectToolChain(false);
  if (mbt_tc->fromMap(data))
    return mbt_tc;
  else
  {
    delete mbt_tc;
    return 0;
  }
}

} // namespace mbtproject

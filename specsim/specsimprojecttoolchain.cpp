#include "specsimprojecttoolchain.hpp"

#include "specsimconstants.hpp"

namespace specsim {

// --------------------------------------------------------------------------
// SpecSimProjectToolChain
// --------------------------------------------------------------------------

SpecSimProjectToolChain::SpecSimProjectToolChain()
  : ProjectExplorer::BaseProjectToolChain(QLatin1String(Constants::SPECSIM_TOOLCHAIN),
                                          QLatin1String(Constants::SPECSIM_PROJECT),
                                          QStringLiteral("SpecSim Project"),
                                          QStringLiteral("Model Simulation Tool Chain"),
                                          true)
{}

SpecSimProjectToolChain::SpecSimProjectToolChain(bool autodetect)
  : ProjectExplorer::BaseProjectToolChain(QLatin1String(Constants::SPECSIM_TOOLCHAIN),
                                          QLatin1String(Constants::SPECSIM_PROJECT),
                                          QStringLiteral("SpecSim Project"),
                                          QStringLiteral("Model Simulation Tool Chain"),
                                          autodetect)
{}

SpecSimProjectToolChain::~SpecSimProjectToolChain()
{}

//// --------------------------------------------------------------------------
//// SpecSimProjectToolChainFactory
//// --------------------------------------------------------------------------

SpecSimProjectToolChainFactory::SpecSimProjectToolChainFactory()
  : ProjectExplorer::BaseProjectToolChainFactory(QLatin1String(Constants::SPECSIM_TOOLCHAIN),
                                                 tr("SpecSim Project"))
{}

SpecSimProjectToolChainFactory::~SpecSimProjectToolChainFactory()
{}

SpecSimProjectToolChain* SpecSimProjectToolChainFactory::create()
{
  return new SpecSimProjectToolChain(true);
}

SpecSimProjectToolChain* SpecSimProjectToolChainFactory::restore(const QVariantMap &data)
{
  SpecSimProjectToolChain* specsim_tc = new SpecSimProjectToolChain(false);
  if (specsim_tc->fromMap(data))
    return specsim_tc;
  else
  {
    delete specsim_tc;
    return nullptr;
  }
}

} // namespace specsim

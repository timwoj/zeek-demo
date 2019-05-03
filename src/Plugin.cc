
#include "Plugin.h"

namespace plugin { namespace Demo_AllCaps { Plugin plugin; } }

using namespace plugin::Demo_AllCaps;

plugin::Configuration Plugin::Configure()
	{
	plugin::Configuration config;
	config.name = "Demo::AllCaps";
	config.description = "<Insert description>";
	config.version.major = 0;
	config.version.minor = 1;
	config.version.patch = 0;
	return config;
	}

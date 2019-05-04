#include "Plugin.h"
#include "CSV.h"

namespace plugin { namespace Demo_AllCaps { Plugin plugin; } }

using namespace plugin::Demo_AllCaps;

Plugin::Plugin()
	{
	EnableHook(HOOK_LOAD_FILE);
        AddComponent(new ::logging::Component("CSV", ::logging::writer::CSV::Instantiate));
	}

plugin::Configuration Plugin::Configure()
	{
	plugin::Configuration config;
	config.name = "Demo::AllCaps";
	config.description = "Capitalize all letters of any string";
	config.version.major = 0;
	config.version.minor = 1;
	config.version.patch = 0;
	return config;
	}

int Plugin::HookLoadFile(const LoadType type, const std::string& file, const std::string& resolved)
	{
	if (type == LoadType::SCRIPT) {
		printf("Loaded script from %s\n", file.c_str());
	}

	return -1;
	}

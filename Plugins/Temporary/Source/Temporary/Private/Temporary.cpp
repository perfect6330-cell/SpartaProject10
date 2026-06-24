#include "Temporary.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogTemporary);

class FTemporaryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		UE_LOG(LogTemporary, Log, TEXT("Temporary plugin module started."));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogTemporary, Log, TEXT("Temporary plugin module shut down."));
	}
};

IMPLEMENT_MODULE(FTemporaryModule, Temporary);

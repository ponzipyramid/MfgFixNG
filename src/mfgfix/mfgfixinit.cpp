#include "mfgfixinit.h"
#include "Settings.h"
#include "BSFaceGenAnimationData.h"
#include "ConsoleCommands.h"
#include "SettingsPapyrus.h"
#include "MfgConsoleFunc.h"
#include "Offsets.h"


namespace MfgFix
{
	void Init()
	{
		Settings::Get().Read();

		BSFaceGenAnimationData::Init();
		ConsoleCommands::Init();

		// Papyrus
		SettingsPapyrus::Register();
		MfgConsoleFunc::Register();

		// Misc

		// allow expression change for dead npcs - 1.5 seems to use short jumps which necessitates a smaller offset ???
		REL::safe_fill(Offsets::BSFaceGenAnimationData::sub_3DB770.address() + REL::VariantOffset(0x4B, 0x4A, 0x4B).offset(), 0x90, REL::VariantOffset(0x09, 0x0D, 0x09).offset()); 
		
		// fix papyrus function SetExpressionOverride that didn't work with aiMood 0
		// first offset seems the same
		REL::safe_write(Offsets::Papyrus::Actor::SetExpressionOverride.address() + 0x001A, static_cast<std::uint16_t>(0x9001));
		// second is different
		REL::safe_write(Offsets::Papyrus::Actor::SetExpressionOverride.address() + REL::VariantOffset(0x29, 0x2A, 0x29).offset(), static_cast<std::uint8_t>(0x10));
		
		// disable code that sometimes cause ctd because of missing check for null parent node
		REL::safe_fill(Offsets::BSFaceGenNiNode::sub_3F0C90.address() + REL::VariantOffset(0x2E3, 0x0395, 0x2E3).offset(), 0x90, 0x0F);
	}
}
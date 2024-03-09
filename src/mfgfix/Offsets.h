#pragma once


namespace MfgFix::Offsets
{

	namespace
	{
		static constexpr REL::RelocationID ModifyFaceGenCommand(22542, 23017);
		static constexpr REL::RelocationID GetExpressionName(26428, 27007);
		static constexpr REL::RelocationID GetModifierName(26429, 27008);
		static constexpr REL::RelocationID GetPhonemeName(26430, 27009);
		static constexpr REL::RelocationID GetCustomName(26431, 27010);
	}

	namespace BSFaceGenAnimationData
	{
		static constexpr REL::RelocationID KeyframesUpdate(25983, 26598);
		static constexpr REL::RelocationID SetExpressionOverride(25980, 26594);
		static constexpr REL::RelocationID sub_3DB770(25979, 26593);
		static constexpr REL::RelocationID Reset(25977, 26586);
	}

	namespace BSFaceGenNiNode
	{
		static constexpr REL::RelocationID sub_3F1800(26417, 26998);
		static constexpr REL::RelocationID sub_3F0C90(26407, 26988);
	}

	namespace Papyrus
	{
		namespace Actor
		{
			static constexpr REL::RelocationID SetExpressionOverride(53926, 54748);
		}
	}
}
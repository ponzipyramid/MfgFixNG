#include "MfgConsoleFunc.h"
#include "BSFaceGenAnimationData.h"
#include "ActorManager.h"

namespace MfgFix::MfgConsoleFunc
{
	namespace
	{
		enum Mode : std::int32_t
		{
			Reset = -1,
			Phoneme,
			Modifier,
			ExpressionValue,
			ExpressionId
		};
	}

	inline bool SetPhoneme(BSFaceGenAnimationData* animData, std::uint32_t a_id, std::int32_t a_value)
	{
		if (!animData) {
			logger::error("No animdata found");
			return false;
		}
		if (a_id < 0 || a_id > 15) {
			logger::error("PhonemeId out of range");
			return false;
		}
		animData->phoneme2.SetValue(a_id, std::clamp(a_value, 0, 200) / 100.0f);
		return true;
	}

	inline std::int32_t GetPhoneme(const BSFaceGenAnimationData& animData, std::uint32_t a_id)
	{
		return a_id < animData.phoneme2.count ? std::lround(animData.phoneme2.values[a_id] * 100.0f) : 0;
	}

	inline bool SetModifier(BSFaceGenAnimationData* animData, std::uint32_t a_id, std::int32_t a_value)
	{
		if (!animData) {
			logger::error("No animdata found");
			return false;
		}
		if (a_id < 0 || a_id > 13) {
			logger::error("ModifierId is out of range");
			return false;
		}
		animData->modifier2.SetValue(a_id, std::clamp(a_value, 0, 200) / 100.0f);

		return true;
	}

	inline std::int32_t GetModifier(const BSFaceGenAnimationData& animData, std::uint32_t a_id)
	{
		return a_id < animData.modifier2.count ? std::lround(animData.modifier2.values[a_id] * 100.0f) : 0;
	}

	inline bool SetExpression(BSFaceGenAnimationData* animData, std::uint32_t a_mood, std::int32_t a_value)
	{
		if (!animData) {
			logger::error("No animdata found");
			return false;
		}
		if (a_mood < 0 || a_mood > 16) {
			logger::error("Mood is out of range");
			return false;
		}
		animData->SetExpressionOverride(a_mood, std::clamp(a_value, 0, 200) / 100.0f);
		return true;
	}

	std::uint32_t GetActiveExpression(const BSFaceGenAnimationData& a_animData)
	{
		std::uint32_t expression = BSFaceGenAnimationData::Expression::MoodNeutral;

		for (std::uint32_t i = 0; i < a_animData.expression1.count; ++i) {
			if (a_animData.expression1.values[i] > a_animData.expression1.values[expression]) {
				expression = i;
			}
		}

		return expression;
	}

	bool SetPhonemeModifierSmooth(RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_mode, std::uint32_t a_id, std::int32_t a_value, float a_speed)
	{
		if (!a_actor) {
			return false;
		}

		auto animData = reinterpret_cast<BSFaceGenAnimationData*>(a_actor->GetFaceGenAnimationData());

		if (!animData) {
			return false;
		}

		ActorManager::SetSpeed(a_actor, a_speed);

		RE::BSSpinLockGuard locker(animData->lock);

		switch (a_mode) {
		case Mode::Reset:
			{
				animData->ClearExpressionOverride();
				animData->Reset(0.0f, true, true, true, false);
				return true;
			}
		case Mode::Phoneme:
			{
				return SetPhoneme(animData, a_id, a_value);
			}
		case Mode::Modifier:
			{
				return SetModifier(animData, a_id, a_value);
			}
		case Mode::ExpressionValue:
			{
				return SetExpression(animData, a_id, a_value);
			}
		}

		return false;
	}

	bool SetPhonemeModifier(RE::StaticFunctionTag* a_tag, RE::Actor* a_actor, std::int32_t a_mode, std::uint32_t a_id, std::int32_t a_value)
	{
		return SetPhonemeModifierSmooth(a_tag, a_actor, a_mode, a_id, a_value, 0.f);
	}

	std::int32_t GetPhonemeModifier(RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_mode, std::uint32_t a_id)
	{
		if (!a_actor) {
			return -1;
		}

		auto animData = reinterpret_cast<BSFaceGenAnimationData*>(a_actor->GetFaceGenAnimationData());

		if (!animData) {
			return -1;
		}

		RE::BSSpinLockGuard locker(animData->lock);

		switch (a_mode) {
		case Mode::Phoneme:
			{
				return a_id < animData->phoneme2.count ? std::lround(animData->phoneme2.values[a_id] * 100.0f) : 0;
			}
		case Mode::Modifier:
			{
				return a_id < animData->modifier2.count ? std::lround(animData->modifier2.values[a_id] * 100.0f) : 0;
			}
		case Mode::ExpressionValue:
			{
				return a_id < animData->expression1.count ? std::lround(animData->expression1.values[a_id] * 100.0f) : 0;
			}
		case Mode::ExpressionId:
			{
				return GetActiveExpression(*animData);
			}
		}

		return -1;
	}

	inline bool ResetMFGSmooth(RE::StaticFunctionTag*, RE::Actor* a_actor, int a_mode)
	{
		if (!a_actor) {
			logger::error("No actor selected");
			return false;
		}
	
		auto animData = reinterpret_cast<BSFaceGenAnimationData*>(a_actor->GetFaceGenAnimationData());

		if (!animData) {
			logger::error("No animdata found");
			return false;
		}

		RE::BSSpinLockGuard locker(animData->lock);

		switch (a_mode) {
		case Mode::Reset:
			{
				// Blinks, Brows, Eyes, Squints
				for (int m = 0; m <= 13; m++) {
					SetModifier(animData, m, 0);
				}

				// Mouth
				for (int p = 0; p <= 13; p++) {
					SetPhoneme(animData, p, 0);
				}
				// Expressions
				SetExpression(animData, GetActiveExpression(*animData), 0);
				break;
			}
		case Mode::Phoneme:
			{
				// Mouth
				for (int p = 0; p <= 13; p++) {
					SetPhoneme(animData, p, 0);
				}
				break;
			}
		case Mode::Modifier:
			{
				// Blinks, Brows, Eyes, Squints
				for (int m = 0; m <= 13; m++) {
					SetModifier(animData, m, 0);
				}
				break;
			}
		default:
			{
				logger::warn("ResetMFGSmooth: unexpected aimode");
				break;
			}
		}
		return true;
	}

	inline bool ApplyExpressionPreset(RE::StaticFunctionTag*, RE::Actor* a_actor, std::vector<float> a_expression, bool a_openMouth, int exprPower, float exprStrModifier, float modStrModifier, float phStrModifier, float a_speed)
	{
		if (!a_actor) {
			logger::error("No actor selected");
			return false;
		}

		if (a_expression.size() != 32) {
			logger::error("Expression is of incorrect size - returning");
			return false;
		}

		auto animData = reinterpret_cast<BSFaceGenAnimationData*>(a_actor->GetFaceGenAnimationData());

		if (!animData) {
			logger::error("No animdata found");
			return false;
		}

		ActorManager::SetSpeed(a_actor, a_speed);
		RE::BSSpinLockGuard locker(animData->lock);

		int i = 0;
		int p = 0;
		int m = 0;

		// Set expression
		int exprNum = static_cast<int>(a_expression[30]);
		int exprStrResult = static_cast<int>(a_expression[31] * 100.0 * exprStrModifier);

		// dynamic exprPower for non angry expressions
		if (exprNum > 0) {
			if (exprStrResult == 0) {
				exprStrResult = exprPower;
			}
		}
		if (!a_openMouth) {
			SetExpression(animData, exprNum, exprStrResult);
		}
		// Set Phoneme
		while (p <= 15) {
			if (!a_openMouth && GetPhoneme(*animData, p) != a_expression[i]) {
				SetPhoneme(animData, p, static_cast<int>(a_expression[i] * 100.0 * phStrModifier));
			}
			++i;
			++p;
		}
		// Set Modifier
		while (m <= 13) {
			if (GetModifier(*animData, m) != a_expression[i]) {
				SetModifier(animData, m, static_cast<int>(a_expression[i] * 100.0 * modStrModifier));
			}
			++i;
			++m;
		}

		return true;
	}



	void Register()
	{
		SKSE::GetPapyrusInterface()->Register([](RE::BSScript::IVirtualMachine* a_vm) {
			a_vm->RegisterFunction("SetPhonemeModifierSmooth", "MfgConsoleFunc", SetPhonemeModifierSmooth);
			a_vm->RegisterFunction("SetPhonemeModifier", "MfgConsoleFunc", SetPhonemeModifier);
			a_vm->RegisterFunction("GetPhonemeModifier", "MfgConsoleFunc", GetPhonemeModifier);
			a_vm->RegisterFunction("ResetMFGSmooth", "MfgConsoleFunc", ResetMFGSmooth);
			a_vm->RegisterFunction("ApplyExpressionPreset", "MfgConsoleFunc", ApplyExpressionPreset);
			return true;
		});
	}
}
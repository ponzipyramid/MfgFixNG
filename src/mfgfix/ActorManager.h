#pragma once
#include "Settings.h"

namespace MfgFix
{
	class ActorManager
	{
	public:
		static inline void SetSpeed(RE::Actor* a_actor, float a_speed)
		{
			if (!a_actor)
				return;

			if (auto animData = a_actor->GetFaceGenAnimationData()) {
				auto id = reinterpret_cast<uintptr_t>(animData);
				auto formId = a_actor->GetFormID();
				
				if (a_speed == 0.0) {
					_mapping.erase(id);
					_speed.erase(formId);
				} else {
					_mapping[id] = formId;
					_speed[formId] = a_speed;
				}
			}
		}

		static inline float GetSpeed(BSFaceGenAnimationData* a_data)
		{
			auto id = reinterpret_cast<uintptr_t>(a_data);
			if (_mapping.count(id)) {
				auto formId = _mapping[id];
				if (_speed.count(formId)) {
					return _speed[formId];
				}
			}

			return Settings::Get().transition.fDefaultSpeed;
		}

	private:
		static inline std::unordered_map<RE::FormID, float> _speed;
		static inline std::unordered_map<std::uintptr_t, RE::FormID> _mapping;
		static inline float _defaultSpeed{ 0.f };
	};
}
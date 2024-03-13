#pragma once

namespace MfgFix
{
	constexpr float DEFAULT_SPEED = 0.2f;

	class ActorManager
	{
	public:
		static inline void SetSpeed(RE::Actor* a_actor, float a_speed)
		{
			if (!a_actor)
				return;

			if (auto animData = a_actor->GetFaceGenAnimationData()) {
				auto formId = a_actor->GetFormID();

				_mapping[reinterpret_cast<uintptr_t>(animData)] = formId;
				_speed[formId] = a_speed;
			}
		}

		static inline float GetSpeed(BSFaceGenAnimationData* a_data)
		{
			auto id = reinterpret_cast<uintptr_t>(a_data);
			if (_mapping.count(id)) {
				logger::info("GetSpeed: found mapping using id {}", id);
				auto formId = _mapping[id];
				if (_speed.count(formId)) {
					return _speed[formId];
				}
			}
			logger::info("GetSpeed: giving default for {}", id);

			return DEFAULT_SPEED;
		}

	private:
		static inline std::unordered_map<RE::FormID, float> _speed;
		static inline std::unordered_map<std::uintptr_t, RE::FormID> _mapping;
	};
}
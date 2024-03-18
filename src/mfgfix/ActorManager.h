#pragma once

#include "BSFaceGenAnimationData.h"

namespace MfgFix
{
	constexpr float DEFAULT_SPEED = 0.0f;

	    class IItemChangeVisitor
	{
	public:
		virtual ~IItemChangeVisitor() {}

		virtual RE::BSContainer::ForEachResult Visit(RE::InventoryEntryData*) { return RE::BSContainer::ForEachResult::kContinue; };        // 01
		virtual bool ShouldVisit([[maybe_unused]] RE::InventoryEntryData*, [[maybe_unused]] RE::TESBoundObject* a_object) { return true; }  // 02
		virtual RE::BSContainer::ForEachResult Unk_03(RE::InventoryEntryData* a_entryData, [[maybe_unused]] void* a_arg2, bool* a_arg3)                 // 03
		{
			*a_arg3 = true;
			return Visit(a_entryData);
		}

		RE::InventoryChanges::IItemChangeVisitor& AsNativeVisitor() { return *(RE::InventoryChanges::IItemChangeVisitor*)this; }
	};
	static_assert(sizeof(IItemChangeVisitor) == 0x8);

	class WornVisitor : public IItemChangeVisitor
	{
	public:
		WornVisitor(std::function<RE::BSContainer::ForEachResult(RE::InventoryEntryData*)> a_fun) :
			_fun(a_fun){};

		virtual RE::BSContainer::ForEachResult Visit(RE::InventoryEntryData* a_entryData) override
		{
			return _fun(a_entryData);
		}

	private:
		std::function<RE::BSContainer::ForEachResult(RE::InventoryEntryData*)> _fun;
	};

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
				
				_mapping[id] = formId;
				if (a_speed == 0.0) {
					_speed.erase(formId);
				} else {
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

			return DEFAULT_SPEED;
		}

		static inline void AddPhonemeBlockKwd(RE::BGSKeyword* a_kwd)
		{
			_phonemeBlocks.insert(a_kwd);
		}

		class WornVisitor
		{
		public:
			WornVisitor(std::function<RE::BSContainer::ForEachResult(RE::InventoryEntryData*)> a_fun) :
				_fun(a_fun){};

			RE::BSContainer::ForEachResult Visit(RE::InventoryEntryData* a_entryData)
			{
				return _fun(a_entryData);
			}

		private:
			std::function<RE::BSContainer::ForEachResult(RE::InventoryEntryData*)> _fun;
		};

		static inline void RemovePhonemeBlockKwd(RE::BGSKeyword* a_kwd)
		{
			_phonemeBlocks.erase(a_kwd);
		}

		static bool IsPhonemeBlocked(RE::Actor* a_actor)
		{
			bool blocked = false;

			std::vector<RE::BGSKeyword*> kwds(_phonemeBlocks.begin(), _phonemeBlocks.end());

			auto visitor = MfgFix::WornVisitor([a_actor, &kwds, &blocked](RE::InventoryEntryData* a_entry) {

				#undef GetObject
				auto obj = a_entry->GetObject();
				if (auto armor = obj->As<RE::TESObjectARMO>()) {
					if (armor->HasKeywordInArray(kwds, false)) {
						blocked = true;
						return RE::BSContainer::ForEachResult::kStop;
					}
				}

				return RE::BSContainer::ForEachResult::kContinue;
			});

			a_actor->GetInventoryChanges()->VisitWornItems(visitor.AsNativeVisitor());

			return blocked;
		}
	private:
		static inline std::unordered_map<RE::FormID, float> _speed;
		static inline std::unordered_map<std::uintptr_t, RE::FormID> _mapping;
		static inline std::unordered_set<RE::BGSKeyword*> _phonemeBlocks;
	};
}
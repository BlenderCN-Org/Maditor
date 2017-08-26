#pragma once

#include "Serialize\Container\set.h"
#include "Serialize\Container\observed.h"

#include "TreeUnit.h"

#include "SignalSlot\slot.h"

namespace Maditor {
	namespace Model {

		class ProfilerItem : public TreeUnitItem<ProfilerItem> {
		public:
			ProfilerItem(ProfilerModel *parent, const std::string &name);
			ProfilerItem(ProfilerItem *parent, const std::string &name);

			// Geerbt über TreeUnitItem
			virtual int childCount() const override;
			virtual ProfilerItem * child(int i) override;
			virtual QVariant data(int col) const override;

			const std::string &key() const;

		protected:
			std::tuple<ProfilerItem*, std::string> createNode(const std::string &name);

			void update(size_t fullDuration);
			void notify();

		private:
			Engine::Serialize::Observed<size_t> mDuration;
			Engine::Serialize::ObservableSet<ProfilerItem, Engine::Serialize::ContainerPolicy::masterOnly, Engine::Serialize::ParentCreator<decltype(&ProfilerItem::createNode), &ProfilerItem::createNode>> mChildren;

			Engine::SignalSlot::Slot<decltype(&ProfilerItem::update), &ProfilerItem::update> mUpdateSlot;

			ProfilerItem *mParent;
			std::string mName;
			float mFullDuration;
		};

		class ProfilerModel : public TreeUnit<ProfilerModel> {
			Q_OBJECT

		public:
			ProfilerModel(Engine::Serialize::TopLevelSerializableUnitBase *topLevel);


			// Geerbt über TreeUnit
			virtual int childCount() const override;

			virtual ProfilerItem * child(int i) override;

			void reset();

		protected:
			virtual QVariant header(int col) const override;

		private:
			std::tuple<ProfilerModel*, std::string> createNode(const std::string &name);

		private:
			Engine::Serialize::ObservableSet<ProfilerItem, Engine::Serialize::ContainerPolicy::masterOnly, Engine::Serialize::ParentCreator<decltype(&ProfilerModel::createNode), &ProfilerModel::createNode>> mTopLevelItems;

			

		};
	}
}
#include "maditormodellib.h"

#include "ProfilerModel.h"

namespace Maditor {
	namespace Model {

		ProfilerModel::ProfilerModel() :
			TreeUnit(4),
			mTopLevelItems(this, &ProfilerModel::createNode)
		{
			setContainer(mTopLevelItems);
		}

		ProfilerItem::ProfilerItem(TreeUnitItem * parent, const std::string & name) :
			TreeUnitItem(parent),
			mName(name),
			mParent(0),
			mChildren(this, &ProfilerItem::createNode),
			mDuration(this, 0),
			mFullDuration(0.0f)
		{
			setContainer(mChildren);
			mDuration.setCallback([=](const size_t &duration) { 
				update(duration);
			});
		}

		ProfilerItem::ProfilerItem(ProfilerItem * parent, const std::string & name) :
			TreeUnitItem(parent),
			mName(name),
			mParent(parent),
			mChildren(this, &ProfilerItem::createNode),
			mDuration(this, 0),
			mFullDuration(0.0f)
		{
			setContainer(mChildren);
		}

		int ProfilerItem::childCount() const
		{
			return mChildren.size();
		}

		ProfilerItem * ProfilerItem::child(int i)
		{
			auto it = mChildren.begin();
			std::advance(it, i);
			return &it->second;
		}

		QVariant ProfilerItem::data(int col) const
		{
			switch (col) {
			case 0:
				return QString::fromStdString(mName);
			case 1:
				return QLocale().toString(mDuration / 20000000.f);
			case 2:
				return mParent && mParent->mDuration > 0 ? 100.f * mDuration / mParent->mDuration : QVariant("-");
			case 3:
				return 100.f * mDuration / mFullDuration;
			}

			return QVariant();
		}

		std::tuple<std::string, ProfilerItem*, std::string> ProfilerItem::createNode(const std::string & name)
		{
			return{ name, this, name };
		}

		void ProfilerItem::update(size_t fullDuration)
		{
			mFullDuration = fullDuration;
			notify();
			//notifyDataChange(3);
			for (std::pair<const std::string, ProfilerItem> &child : mChildren) {
				child.second.update(fullDuration);
			}
		}

		void ProfilerItem::notify()
		{
			notifyDataChange(1, 3);
		}

		int ProfilerModel::childCount() const
		{
			return mTopLevelItems.size();
		}

		ProfilerItem * ProfilerModel::child(int i)
		{
			auto it = mTopLevelItems.begin();
			std::advance(it, i);
			return &it->second;
		}

		void ProfilerModel::reset()
		{
			mTopLevelItems.clear();
		}

		inline QVariant ProfilerModel::header(int col) const
		{
			switch (col) {
			case 0:
				return "Name";
			case 1:
				return "Total Time (ms)";
			case 2:
				return "Rel. Time (parent)";
			case 3:
				return "Rel. Time (total)";
			default:
				return QVariant();
			}
		}

		std::tuple<std::string, TreeUnitItem*, std::string> ProfilerModel::createNode(const std::string & name)
		{
			return{ name, this, name };
		}

	}
}
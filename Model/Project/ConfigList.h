#pragma once


#include "ProjectElement.h"

#include "Generators\CmakeProject.h"

#include "Module.h"

#include "ApplicationConfig.h"

namespace Maditor {
	namespace Model {
		class MADITOR_MODEL_EXPORT ConfigList : public QObject, public ProjectElement{
			Q_OBJECT

		public:

			ConfigList(Project *parent);
			ConfigList(QDomElement element, Project *parent);
			ConfigList(const ConfigList &) = delete;
			~ConfigList();			

			virtual QString path() const override;

			bool hasConfig(const QString &name) const;

			ApplicationConfig *getConfig(const QString &name);
			const ApplicationConfig *getConfig(const QString &name) const;


			std::list<std::unique_ptr<ApplicationConfig>>::const_iterator begin() const;
			std::list<std::unique_ptr<ApplicationConfig>>::const_iterator end() const;

			virtual QVariant icon() const override;

			// Inherited via ProjectElement
			virtual int childCount() const override;

			virtual ApplicationConfig * child(int i) override;

			QStringList getConfigs();

		public slots:
			void createConfig(const QString &name);

		private:
			void init();

			void addConfig(ApplicationConfig *module);
			void newConfig();

		signals:
			void configAdded(ApplicationConfig *);		
			void instanceAdded(ApplicationLauncher *);
			void instanceDestroyed(ApplicationLauncher *);

			void openConfig(ApplicationConfig *);

		private:
			Project *mParent;

			std::list<std::unique_ptr<ApplicationConfig>> mConfigs;


		};
	}
}
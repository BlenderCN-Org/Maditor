#pragma once


namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT Log : public QObject {
			Q_OBJECT

		public:

			enum Type {
				TextLog,
				GuiLog
			};

			Log(const std::string &name, Type type);
			virtual ~Log();

			Type type() const;
			const std::string &name() const;
			LogTableModel *model();

		signals:
			void messageReceived(const QString &msg);

		private:
			std::unique_ptr<LogTableModel> mModel;
			std::string mName;
			Type mType;

		};

	}
}
#pragma once



namespace Maditor {
	namespace Model {

		class Document;

		class Maditor;
		class Project;
		class ApplicationConfig;
		class ApplicationLauncher;
		class OgreWindow;

		class Module;
		class ModuleList;

		class ConfigList;

		class ModuleLoader;

		class InputWrapper;

		class TreeItem;
		class TreeUnitBase;
		class TreeModel;

		class ProfilerModel;
		class StatsModel;

		class LogsModel;
		class Log;
		class LogTableModel;

		class DialogManager;

		namespace Generators {
			class CmakeSubProject;
			class ClassGenerator;
			class ServerClassGenerator;
		}

	}

	namespace View {
		class MainWindow;
		namespace Dialogs {
			class SettingsDialog;
		}
		namespace Ui {
			class MainWindow;
		}
	}

	namespace Addons {
		class AddonCollector;
	}

}
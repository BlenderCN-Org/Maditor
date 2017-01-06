#pragma once



namespace Maditor {
	namespace Model {

		class Maditor;
		class Project;
		class ApplicationLauncher;
		class OgreWindow;

		class Module;
		class ModuleList;

		class ModuleLoader;

		class InputWrapper;

		class TreeItem;
		class TreeUnit;
		class TreeModel;

		class ProfilerModel;

		class LogsModel;
		class Log;
		class LogTableModel;

		class DialogManager;

		namespace Generators {
			class CmakeSubProject;
			class ClassGenerator;
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
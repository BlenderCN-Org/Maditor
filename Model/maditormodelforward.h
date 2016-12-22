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
	}

	namespace Addons {
		class AddonCollector;
	}

}
#pragma once

#include "Model/TableModel.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {


		class TableUnitBase : public TableModel {
			Q_OBJECT

		public:
			
			TableUnitBase(int columnCount);

			
		protected:
			template <class C>
			void setContainer(C &container) {
				container.connectCallback([&container = (const C&)container, this](const typename C::const_iterator &it, int op) {
					int row = std::distance(container.begin(), it);
					handleOperation(row, op);
				});
			}

			void handleOperation(int row, int op);
			
		};

		template <class T>
		class TableUnit : public TableUnitBase, public Engine::Serialize::SerializableUnit<T> {
		public:
			using TableUnitBase::TableUnitBase;
		};

	}
}
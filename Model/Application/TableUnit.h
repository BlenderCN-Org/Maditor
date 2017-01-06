#pragma once

#include "Model/TableModel.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {


		class TableUnit : public TableModel, public Engine::Serialize::SerializableUnit {
			Q_OBJECT

		public:
			
			TableUnit(int columnCount);

			
		protected:
			template <class C>
			void setContainer(C &container) {
				container.setCallback([&container = (const C&)container, this](const typename C::const_iterator &it, int op) {
					int row = std::distance(container.begin(), it);
					handleOperation(row, op);
				});
			}

			void handleOperation(int row, int op);
			
		};
	}
}
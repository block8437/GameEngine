#include "dynamic_object.h"
#include "static_object.h"
#include <vector>
class RenderList {
	private:
		std::vector<DynamicObject*> dynamicObjectList;
		std::vector<StaticObject*> staticObjectList;

	public:
		RenderList();
		void add(DynamicObject* obj);
		void add(StaticObject* obj);

		void render();
		void clean();
};
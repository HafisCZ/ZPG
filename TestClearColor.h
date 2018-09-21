#pragma once

#include "Test.h"

#include <GL/glew.h>

#include "vendor/imgui.h"

namespace test {

	class ClearColor : public Test {
		public:
			ClearColor();
			~ClearColor();

			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onImGuiRender() override;

		private:
			float m_clearColor[4];
	};

}
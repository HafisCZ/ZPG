#pragma once

#include <vector>
#include <functional>

namespace test {

	class Test {
		public:
			Test() {}
			virtual ~Test() {}

			virtual void onUpdate(float deltaTime) {}
			virtual void onRender() {}
			virtual void onImGuiRender() {}
	};

	class TestMenu : public Test {
		public:
			TestMenu(Test*& currentTestPointer);

			template<typename T> void registerTest(const std::string& name) {
				m_tests.push_back(std::make_pair(name, []() { return new T(); }));
			}

			virtual void onImGuiRender() override;
		private:
			Test*& m_currentTest;
			std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
	};
}
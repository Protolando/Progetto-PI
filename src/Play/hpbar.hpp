#ifndef HPBARINC
#define HPBARINC

#include "../Context/Views/views.hpp"

class HpBar : public GGView{
	public:
		HpBar(int, glm::vec2*);
		HpBar(int, glm::vec2*, int, int);
		~HpBar();
		
		void draw() override;
		bool wasClicked(GGClickEvent*) override;
		
		void setHp(int);
		void changeHp(int);
		void setPosition(glm::vec2*);
		int getHeight();
		int getWidth();
		void setWidth(int);
		void setHeight(int);
	private:
		glm::vec2 *position;
		int tothp;
		int currhp;
		
		int width;
		int height;
};

#endif

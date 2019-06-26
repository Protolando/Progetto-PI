#ifndef SELECTMAPINC
#define SELECTMAPINC

#include "../Context/Views/views.hpp"

class SelectMap : public GGView{
public:
	SelectMap();
	~SelectMap();
	
	void draw() override;
	bool onMouseUp(GGClickEvent *) override;
	void onResize(GGResizeEvent *) override;
	void onMouseMove(GGMouseMoveEvent *) override;
private:
	Lable *selectMapLable;
	Button *backButton;
	int mapNamesWidth;
	std::vector<Lable *> map_list;
	bool wasClicked(GGClickEvent *) override;
};

#endif

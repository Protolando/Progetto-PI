subdir('Context')
subdir('Create')
subdir('Play')

mainmenulib = static_library('mainmenulib',
			['mainmenu.cpp',
			 'mainmenu.hpp'
			],
			link_with : [createviewlib, playviewlib, contextlib],
			install : true)
			
#Creating executable
executable('Progetto_PI', 'main.cpp',
	dependencies : deps,
	include_directories : [],
	link_with : [contextlib, mainmenulib],
	install : true
)

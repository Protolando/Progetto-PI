conf = configuration_data()
conf.set_quoted('PROJECT_PATH', join_paths(get_option('prefix'), projdir))
configure_file(
  output: 'config.h',
  configuration: conf
)

subdir('Events')
subdir('Views')
subdir('soil')

contextlib = static_library('contextlib',
                        ['context.hpp',
                         'context.cpp',
                         'renderer.hpp',
                         'renderer.cpp',
                         'rect.hpp',
                         'rect.cpp'
			],
			link_with : [eventslib, viewslib, soillib],
                        install : true)

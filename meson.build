project('Progetto_PI', ['cpp', 'c'],        version: '0.1.0',
  meson_version: '>= 0.40.0',
)

glDep = dependency('GL')
glfwDep = dependency('glfw3')
glewDep = dependency('glew')
freetypeDep = dependency('freetype2')

deps = [glDep, glfwDep, glewDep, freetypeDep]

projdir = join_paths(get_option('datadir'), 'guns_gameexports')

conf = configuration_data()
conf.set_quoted('PROJECT_PATH', join_paths(get_option('prefix'), projdir))
configure_file(
  output: 'config.h',
  configuration: conf
)

subdir('fonts')
subdir('src')

# SConstruct

env = Environment(
    CXX='C:/msys64/ucrt64/bin/g++.exe',
    CC='C:/msys64/ucrt64/bin/gcc.exe',
    CPPPATH=['C:/msys64/ucrt64/include'],
    LIBPATH=['C:/msys64/ucrt64/lib'],
    LIBS=['hidapi'],
    CXXFLAGS=['-fdiagnostics-color=always', '-g'],
)

sources = [
    'main.cpp',
    'helpers.cpp',
    'joycon.cpp',
    'shared_joycons.cpp',
]

env.Program(
    target='main.exe',
    source=sources
)

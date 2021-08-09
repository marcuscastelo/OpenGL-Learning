# OpenGL-Learning
My documented experience through learning OpenGL (code commented with my understanding)

## Resources I am following to learn OpenGL

- [TheCherno's OpenGL series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)
- [Documentation on docs.GL](https://docs.gl)

## How to run (Tested on Arch Linux x86_64)

First, clone the repository with all it's submodules (GLFW and GLEW):

` git clone --recursive https://github.com/marcuscastelo/OpenGL-Learning `

Then, enter _dependencies_ folder and build each dependency according to their guides below:

[GLFW Compilation Guide](https://www.glfw.org/docs/latest/compile_guide.html) 
(create a `dependencies/glfw/build` folder, and build out-of-tree there)

[GLEW Compilation Guide](http://glew.sourceforge.net/build.html)
(GLEW should be a little bit more straight-forward, since it already put the files in the right folders `dependencies/glew/lib` and `dependencies/glew/include`)

After building the dependencies, just run `make run` on the repo root folder.

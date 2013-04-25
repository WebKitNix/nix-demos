#About PyNIX#

These are very simple Python bindings for Nix, they were made
using Python's C API. A demo Python script that uses the bindings
is also provided.

#Dependencies#

You'll need the OpenGL module for Python. On Ubuntu do this:

<pre>
  sudo apt-get install python-opengl
</pre>

#Compiling#

Create a build directory inside glut-nix-view base directory and change to it:
<pre>
  mkdir build
  cd build
</pre>

Inform where the dependencies are located:
<pre>
  export LD_LIBRARY_PATH=$WEBKITOUTPUTDIR/install/lib
</pre>

Compile it using **cmake**:
<pre>
  cmake ..
  make
</pre>

#Running#

<pre>
  ../pynixdemo
</pre>

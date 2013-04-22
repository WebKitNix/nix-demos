#About glut-nix-view#

This is a minimal use of nix with GLUT.
This example also includes a minimal example
of how to inject functions into Javascript.

#Compiling#

Create a build directory inside glut-nix-view base directory and change to it:
<pre>
  mkdir build
  cd build
</pre>

Inform where the dependencies are located:
<pre>
  export PKG_CONFIG_PATH=/opt/nix/lib/pkgconfig/:/opt/nix-deps/lib/pkgconfig
</pre>

Compile it using **cmake**:
<pre>
  cmake ..
  make
</pre>

#Running#

<pre>
  ./GlutView file:///$PWD/test.html
</pre>

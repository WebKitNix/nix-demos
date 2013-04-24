#About cpumonitor#

This is a Nix example that uses an InjectedBundle to
provide some methods to Javascript. This way it can
show a UI that monitors CPU usage (per core).

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
  ./CPUMonitor
</pre>

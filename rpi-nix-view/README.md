#About rpi-nix-demo#

This demo opens a webview pointing to google.com.

#Compiling#

rpi-nix-demo needs nix-deps and nix libraries to be compiled. Compile
them and install prior compiling this demo.

Create a build directory inside rpi-nix-demo base directory and change to it:
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

#Running on raspberry-pi#

Copy the entire rpi-nix-demo to the rpi board. Ensure that you have all the
dependencies properly installed (nix-deps and nix). Run it:
<pre>
  ./run_sample.sh
</pre>

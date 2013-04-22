#!/bin/bash
LD_LIBRARY_PATH=/opt/nix/lib:/opt/nix-deps/lib build/Sample ${1:-"http://www.google.com"}

UE4 Quake .map Importer
===============================

A UE4 plugin to import Quake style .map files (Quake 1, HL 1 and Q3) and convert them into UE4 brush geometry.
Note this is not a .bsp importer, instead it load the source .map files.

What works:
====
* Loading and parsing Q1 maps brush data
* Loading the plane brush data from Q3 format maps
* Calulating the plane intersections

TODO:
====
* Prune Verts that are outside of the brush
* Sort the verts for a face correctly
* Add suport for spawning some of the enitys, eg. lights and other ones that make sense
* Parse and apply UVS
* Add suport for HL1 format maps
* UI and menus
* Cose tidy ups

Current Progress Image:
====
<img src="http://waffo.net/arkii/screens/2014-11-19_07-24-57.png" />

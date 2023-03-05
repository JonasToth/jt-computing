#!/usr/bin/env gnuplot

set terminal pngcairo truecolor size 640,640 background rgb "#202020" font "Arial"
set font "Helvetica"
set key off
unset border
unset xtics
unset ytics
unset colorbox
set palette rgbformulae 18,12,15
set palette maxcolors 16
set size square

set output "example_000.png"
plot "example_000.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_005.png"
plot "example_005.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_010.png"
plot "example_010.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_015.png"
plot "example_015.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_020.png"
plot "example_020.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_025.png"
plot "example_025.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_030.png"
plot "example_030.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_035.png"
plot "example_035.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_040.png"
plot "example_040.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_045.png"
plot "example_045.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_050.png"
plot "example_050.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_055.png"
plot "example_055.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_060.png"
plot "example_060.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_065.png"
plot "example_065.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_070.png"
plot "example_070.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_075.png"
plot "example_075.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_080.png"
plot "example_080.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_085.png"
plot "example_085.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_090.png"
plot "example_090.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_095.png"
plot "example_095.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

set output "example_100.png"
plot "example_100.dat" using 1:2:3 with points ps 3.8 pt 5 palette notitle

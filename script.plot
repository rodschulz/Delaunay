unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xrange [-5:25]
set yrange [-5:14]

set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

#set term png
#set output "output.png"

plot "triangulation_0.dat" every :::0::100 with lp title 'Triangulación', "points.dat" every :::0::0 using 1:2 with lp pointtype 4 title 'Triangulación inicial', "" every :::1::1 title 'Puntos'

pause -1

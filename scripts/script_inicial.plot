unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xrange [-20:40]
set yrange [-10:40]

set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

set term png
set output "initial_triangulation.png"

plot "inicial.dat" every :::0::0 with lp pointtype 3 title 'Rectangulo', "" every :::1::1 with lp title 'Triangulo', "" every :::2::2 title 'Puntos'

pause -1

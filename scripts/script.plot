unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xrange [-20:40]
set yrange [-10:40]

set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

set term png

set output "t0.png"
plot "triangulation_0.dat" every :::0::100 with lp title 'Triangulación'

set output "t1.png"
plot "triangulation_1.dat" every :::0::100 with lp title 'Triangulación'

set output "t1_flip.png"
plot "triangulation_1_flip.dat" every :::0::100 with lp title 'Triangulación'

set output "t2.png"
plot "triangulation_2.dat" every :::0::100 with lp title 'Triangulación'

set output "t3.png"
plot "triangulation_3.dat" every :::0::100 with lp title 'Triangulación'


pause -1

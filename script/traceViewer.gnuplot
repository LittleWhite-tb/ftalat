set autoscale x
set autoscale y

set xlabel "Iteration number"
set ylabel "Kernel latency"

# set xrange[1000 to 70000]
# set ytics ("0ms" 0, "1ms" 1, "10ms" 10)

set log y
set yrange [0.1 to 12]
# set ytics ("0ms" 0, "1ms" 1, "10ms" 10)
set format y "%.0f us"

set term png size 640,480
set output "results.png"

plot "../results.dump" using ($1/3400) axes x1y1 title "latency" with linespoints

set term wxt

plot "../results.dump" using ($1/3400) axes x1y1 title "latency" with linespoints

set terminal postscript enhanced color
set output '| ps2pdf - results.pdf'

plot "../results.dump" using ($1/3400) axes x1y1 title "latency" with linespoints

set term pdfcairo enhanced color
set output "results2.pdf"

plot "../results.dump" using ($1/3400) axes x1y1 title "latency" with linespoints

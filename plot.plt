set terminal pngcairo size 1366,768 font 'Verdana,10'
set object 1 rectangle from screen 0,0 to screen 1,1 fillcolor rgb"#111111" behind
set border 31 lc rgb"#DDDDDD"

set output "output.png"
set timefmt "[%Y-%m-%d %H:%M:%S]"
set xdata time
set format x "%H:%M"
set datafile separator " "
set grid mxtics xtics ytics mytics lt -1 lc rgb"#333333"
set xtics 3600
set mxtics 6
set yrange [0:100000]
set ytics 10000
set mytics 2
# set xtics 3600*24 rotate by 90 right

set xlabel "time"
set ylabel "milliwatts"

# set style line 1 lt rgb "#FF0000" lw 1
# set style line 2 lt -1
# set style line 1 lt rgb "#FF0000" lw 1
# set style line 2 lt rgb "#00A000" lw 1
# set key off
# set xrange ["2014-11-02 19:20:00":]
#plot "logfile.txt" using 1:7 title 'test1' w lines, \
#     "logfile.txt" using 1:6 title 'test' w lines, \
#     "logfile.txt" using 1:($2*10) title 'test' w lines, \
#     "logfile.txt" using 1:($5*7) title 'test' w lines, \
#     "logfile.txt" using 1:9 title 'test' w lines
#    "acpi.log" using 1:($3/1000):timestamp($1) with labels font "Verdana,10" right tc lt 2

plot "bat.log" using 1:($3/1000) title "bat.log" w lines
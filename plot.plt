set terminal pngcairo size 1920,1080 enhanced font 'Verdana,10'
#set terminal svg size 1920,1080 fname 'verdana' fsize 10
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
set yrange [0:55000]
set ytics 5000
set mytics 2

set xlabel "time" textcolor rgb"#FFFFFF"
set ylabel "milliwatts" textcolor rgb"#FFFFFF
set key tc rgb "#FFFFFF"

set style line 1 lt rgb "#00FF00" lw 1
set style line 2 lt rgb "#0000FF" lw 1

plot "bat.log" using 1:($3/1000) title "Current capacity" w lines ls 1,\
     "bat.log" using 1:($4/1000) title "Max capacity" w lines ls 2
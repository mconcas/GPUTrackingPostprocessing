set title "Per TF wall time"
set xlabel "TF index"
set ylabel "Wall Time (s)"
set grid
set term png
set output "time_comparison.png"

# Preprocess data with awk
!awk -F' ' '/GPU Reconstruction time for this TF/ {print count++ " " $12}' cuda.log > tf_time_cuda.dat
!awk -F' ' '/GPU Reconstruction time for this TF/ {print count++ " " $12}' hip.log > tf_time_hip.dat
!awk -F' ' '/CPU Reconstruction time for this TF/ {print count++ " " $12}' cpu.log > tf_time_cpu.dat

# Plotting the data
plot "tf_time_cpu.dat" using 1:2 with linespoints title 'CPU', "tf_time_cuda.dat" using 1:2 with linespoints title 'CUDA', "tf_time_hip.dat" using 1:2 with linespoints title 'HIP'

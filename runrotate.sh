out=out.pgm
in=student_samples/sample_image_01.pgm

make all
./pgmrotate $in $out

if [ $# -eq "1" ]; then
  if [ $1 = "open" ]; then
    open $out
  fi
fi


echo "Testing my renderer:"

time for i in {1..50}; do ./cmake-build-debug/NEA > /dev/null; done

echo "Testing Inkscape renderer:"

time for i in {1..50}; do inkscape benchmark.svg --export-filename out.png 2&> /dev/null; done
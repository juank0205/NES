function pause(){
 read -s -n 1 -p "Press any key to continue . . ."
 echo ""
}

vendor/bin/premake/premake5 --cc=clang --verbose gmake
pause
